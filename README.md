# Handoff：winmm.dll 代理注入 + 特征码扫描 Hook 框架

> 本文档面向接手此项目的下一个 agent（人或 AI）。读完它，你应该能：
> 1. 理解本项目的核心机制（DLL 代理、特征码扫描、mid-function inline hook）
> 2. 知道开发过程中踩过的坑，避免重蹈
> 3. 把这套方案迁移到其他 IL2CPP / Unity 游戏，或重构为更通用的 hook 框架

配套背景文档（必读）：
- `6月到8月游戏密钥hook点迁移总结.md` — 跨版本密钥 hook 迁移方法论
- `提示词.txt` — 项目原始需求

---

## 1. 项目目标

游戏（《蔚蓝档案》PC，IL2CPP 编译）每次大版本更新后 SQLCipher 数据库密钥变化，hook 点地址漂移。本项目的目标：**winmm.dll 代理注入后，自动扫描特征码定位 hook 点，在控制台打印密钥，无需人工 CE 操作。**

最终成果：dll 放进游戏目录，启动游戏即自动输出 `PRAGMA key = "x'...'";`，游戏正常运行不崩。

---

## 2. 核心技术路线

### 2.1 winmm.dll 代理注入（已有基础设施）

`winmm.dll` 是 Windows 早期加载的 KnownDLL，游戏进程启动即加载。本项目把伪造的 `winmm.dll` 放进游戏目录，它：
- `DllMain` 的 `DLL_PROCESS_ATTACH` 时 `LoadLibraryA` 系统目录的真 winmm.dll，把所有导出函数指针存到 `Orignal*` 全局变量
- `common.asm` 里每个 `Fake*` 导出是 `jmp [Orignal*]` 跳板，本 dll 处理加载时序，真逻辑全转发给系统 dll
- `DLL_PROCESS_DETACH` 时 `FreeLibrary`

这套代理转发是"地基"，hook 逻辑挂在这个地基上。

### 2.2 特征码扫描定位 hook 点（本项目新增）

由于跨版本地址漂移、不可简单加偏移推算（见背景文档 §2.4），用一条**跨版本验证唯一命中**的通配 AOB 在 GameAssembly.dll 内扫描定位：

```
31 D2 E8 ?? ?? ?? ?? 48 89 C2 E8 ?? ?? ?? ?? 48 89 C3 48 8B 05 ?? ?? ?? ?? 48 8B 80 B8 00 00 00 48 8B 50 20
```

`winmm.cpp` 的 `PatternScan` / `ParsePattern`：
- 支持 `??` 通配，mask=0 表通配位
- 只扫模块的可执行 section（解析 PE 头）
- 用首字节快速筛 + memcmp 校验
- 唯一命中才装 hook；多命中告警放弃

`keyScanThread`（DllMain 启动）：
- 每 1 秒查 `GetModuleHandleA("GameAssembly.dll")`
- 宿主进程校验：只在 `BlueArchive.exe` 下才弹控制台、启动扫描（其他进程加载本 dll 时静默跳过）
- 命中 → 装 hook；模块在但 10 轮未命中 → 放弃

### 2.3 Mid-function inline hook（本项目新增，最关键也最坑）

`AutoHookHelper`（Capstone+Keystone trampoline）只适合 hook 函数入口，hook 函数中间位置时**会踩 3 个坑**（见 §3）。本项目改为**手工搬移指令 + 自定义 stub**。

#### Hook 点选择

AOB 命中点 +0x0F（`mov rbx, rax` 之前），此时 **RAX = 64 字符 hex 密钥串**（IL2CPP String）。hook 入口借 RAX 拿密钥。

#### Patch（5 字节，零寄存器破坏）

```
E9 rel32   ; jmp relay
```
不碰任何寄存器/栈，只偷 10 字节（两条指令：`mov rbx,rax` + `mov rax,[rip+disp32]），剩 5 字节 NOP。

#### Relay 页（在 GameAssembly ±2GB 内分配）

`VirtualAlloc` 时以 GameAssembly 基址为 hint 向下扫描 64KB 步长，保证落在 ±2GB 内（rel32 可达）。页内布局：
- offset 0: `jmp [rip+0]` → keyHookStub（绝对寻址，跨 dll 距离不限）
- offset 32: trampoline

#### Trampoline（手工重定位 rip-relative 指令）

被盗 2 条指令搬到 trampoline：
- `mov rbx, rax` — 原样复制
- `mov rax, [rip+disp32]` — **重算 disp32**，使目标仍指向原 KeyData 类指针全局变量

末尾 `jmp [rip+0]` → hookAddr+10（原始未动指令处）。

#### KeyHookStub（common.asm，全量保存/恢复）

```asm
keyHookStub proc
    mov keyCapturedRax, rax        ; 保存 RAX 给 C handler
    mov gSavedRsp, rsp             ; 全局变量保存 RSP（不能用寄存器！见 §3.3）
    push rax / rcx / rdx / r8 / r9 / r10 / r11    ; 全量保存 caller-saved
    sub rsp, 28h                   ; shadow + 对齐
    call keyLogHandler
    add rsp, 28h
    pop r11 ... pop rax            ; 逆序恢复
    mov rsp, gSavedRsp
    jmp [gKeyTrampoline]
keyHookStub endp
```

`keyLogHandler`（C 函数，__try/__except 包裹）：
- 读 IL2CPP String：长度 int32 在 `+0x10`，UTF-16LE 字符在 `+0x14`
- `printf` 输出密钥

#### 宿主进程校验

`keyScanThread` 开头 `IsBlueArchiveHost()`：取主程序名 `_stricmp` 比 `BlueArchive.exe`，不符直接 return，不弹控制台不扫描。这避免了 dll 被其他进程偶然加载时的副作用。

---

## 3. 开发中踩过的坑（重点！）

### 3.1 Capstone/Keystone trampoline 对 rip-relative 指令的 bug

`AutoHookHelper` 的 trampoline 用 Keystone 重汇编 Capstone 文本，但 **Capstone 对 rip-relative 指令打印绝对地址**（如 `[rip + 0x18CF23D40]`），**Keystone 却按字面位移汇编**——被搬走的 `mov rax,[rip+KeyData类指针]` 在 trampoline 里指向错误地址，一执行就崩。

根因：不是框架 bug，是 capstone 输出文本与 keystone 输入约定不匹配。`AutoHookHelper` 完全无法胜任 mid-function hook。

**对策**：所有"含 rip-relative 指令"的 hook 点必须手工搬移并重算 disp32，不能用 `AutoHookHelper`。

### 3.2 trampoline 复制了未被盗的指令

第一次写 trampoline 时，把 `mov rax,[rax+0xB8]` 也复制进去，然后跳回 hookAddr+10。结果这条指令被**执行两次**：rax 变成 `[static_fields+0xB8]`（≈0），紧接着原地址 `mov rdx,[rax+0x20]` 解引用 null 直接崩。

**对策**：trampoline 只搬"被盗指令"，回跳点必须是"原始未动指令"的起点。这里只偷 10 字节就只搬 2 条。

### 3.3 用 caller-saved 寄存器保存 RSP

stub 最初用 `mov r10, rsp` 保存栈指针，handler 返回后 `mov rsp, r10`。**r10 是 caller-saved**，printf/MessageBoxA 返回后 r10 已被破坏，rsp 被设成垃圾值，`pop rax` 从野地址读取 → 崩。

这一坑最隐蔽——寄存器保存/恢复看起来天衣无缝，但"用来保存栈指针的那个寄存器"本身救不了自己。

**对策**：用全局变量保存 RSP。本方案最终版还顺手保存了所有 caller-saved 寄存器，handler 怎么写都不怕。

### 3.4 校验数组字节对齐写错

校验 hook 点字节时，把 `mov rax,[rip+disp32]` 的 4 个通配 disp32 字节"跳过"后仍按连续 memcmp 比对，导致从 +15 起拿 disp 字节和 `48 8B 80 B8...` 硬比，校验恒失败、hook 没装上。

**对策**：分段校验，不要假设连续内存布局。

### 3.5 stub 入口用 movabs 破坏 RAX

第一版用 `movabs rax, stub; push rax; ret` 进 stub，但 hook 点选的是 RAX=密钥的位置，跳转瞬间 RAX 被覆盖——还没拿密钥就丢了。

**对策**：patch 用 `E9 rel32`（零寄存器影响），进 stub 时 RAX 完好。

---

## 4. 文件清单

| 文件 | 角色 |
|---|---|
| `winmm.cpp` | 代理转发 + 扫描线程 + hook 安装 + handler |
| `common.asm` | MASM：winmm 导出 thunk + keyHookStub |
| `ASM_LIB/AutoAssembler.cpp/.h` | Capstone+Keystone 引擎封装，**只用于函数入口 hook** |
| `winmm.def` | 本 dll 导出表（Fake* 名） |
| `6月到8月游戏密钥hook点迁移总结.md` | 跨版本迁移方法论，含特征码与机制分析 |

---

## 5. 验证流程

1. VS 2019：编译 `winmm-proxy.vcxproj` x64 Release
2. 产物 `x64\Release\winmm.dll` 放游戏目录
3. 启动游戏，预期控制台：
   ```
   [KeyHook] waiting for GameAssembly.dll...
   [KeyHook] AOB unique hit: GameAssembly.dll+0x... (VA ...)
   [KeyHook] relay page at ... (delta ... MB from hook)
   [KeyHook] hook installed at GameAssembly.dll+0x..., waiting for key...
   [KeyHook] hook fired, RAX=...
   [KeyHook] PRAGMA key = "x'...'";
   ```
4. 密钥拼成 `PRAGMA key = "x'...'";` 用 sqlcipher 验证能开 ExcelDB.db
5. 游戏正常进主界面（hook 未破坏原执行流）

CE 调试手段（ce-debug 技能）：挂起进程后读 hook 点字节、relay/trampoline 内容、stub 反汇编，逐段核对。

---

## 6. 迁移到其他游戏

### 6.1 保留部分

- winmm.dll 代理转发机制（任何游戏通用）
- `PatternScan` / `ParsePattern` 扫描器
- `keyHookStub` 全量保存/恢复模板（hook 点换寄存器只改 `mov keyCapturedRax, rax` 那一行）
- relay 页分配逻辑（±2GB 内搜索）

### 6.2 需重做的部分

1. **AOB 特征码**：新游戏/新版本重新逆向，找一段跨版本稳定的指令形状做通配签名（背景文档 §3 有方法论）
2. **hook 点偏移**：在 AOB 命中后偏移多少字节下钩，取决于具体指令布局
3. **被盗指令重定位**：hook 点附近若有 rip-relative 指令，手工重算 disp32（这是迁移成本主要来源）
4. **handler 逻辑**：读什么寄存器、按什么布局解密数据，因目标而异
5. **宿主进程校验**：改 `_stricmp` 比的目标进程名

### 6.3 迁移检查清单

- [ ] 特征码在新版本/新游戏唯一命中（多命中说明签名太短）
- [ ] hook 点选在"目标值已就绪、且之后不依赖被破坏寄存器"的位置
- [ ] 盗走的每条指令都搬进 trampoline，rip-relative 的 disp32 重算
- [ ] 回跳点是"原始未动指令"起点，不重执行被盗指令
- [ ] RSP 用全局变量保存
- [ ] handler 用 __try/__except 包裹读内存

---

## 7. 重构为通用 hook 框架的建议

如果未来要做成可维护的通用框架，建议方向：

1. **抽象 HookPoint 描述**：把"AOB + offset + 被盗指令列表 + 寄存器捕获目标"做成数据结构，安装时自动重定位 rip-relative 指令，吐出 trampoline
2. **统一 stub 生成**：全量保存/恢复 caller-saved + 全局变量保存 RSP，由生成器自动产生 asm（或直接用 C++ 写裸字节 patch）
3. **解耦代理与 hook**：代理转发是独立机制，hook 引擎可单独成库，代理 dll 仅作注入载体
4. **多 hook 点管理**：当前实现假设单点 hook，扩展场景需要支持多个 hook 安装/卸载的生命周期

当前 `keyScanThread` + `InstallKeyHook` + `keyHookStub` 已经是一个"可工作的 mid-function hook 引擎"雏形，提炼出 relay 页分配、指令重定位、stub 生成三块即可泛化。

---

## 8. 密钥归档

测试期间实测拿到的密钥（仅本地解包研究用途）：

| 日期 | 密钥（64 hex） | hook RVA |
|---|---|---|
| 2026-11 | `05f508236ff3dc4e5dc7cb6fcea0ede0d90a89753dc5e5d36c30393e9b072f48` | GameAssembly.dll+0x1BFC39E |

> 当前实测密钥与背景文档归档的 2026-08 密钥不同，说明这是更新的版本。每次游戏更新需重新跑一次拿新密钥。