# Handoff：winmm.dll 代理注入 + 特征码扫描 + MinHook 二开 Mid-Function Hook 框架

> 本文档面向接手此项目的下一个 agent（人或 AI）。读完它，你应该能：
> 1. 理解本项目的核心机制（DLL 代理、特征码扫描、基于 MinHook 二开的 mid-function inline hook）
> 2. 知道开发过程中踩过的 8 个坑（§3，重点），避免重蹈
> 3. 把这套方案迁移到其他 IL2CPP / Unity 游戏

配套背景文档（必读）：
- `6月到8月游戏密钥hook点迁移总结.md` — 跨版本密钥 hook 迁移方法论、AOB 特征码来源
- `handoff.md` — 第一代手写 hook 实现的历史记录（已被 MinHook 方案取代，坑位分析仍有效）
- `提示词.txt` — 项目原始需求

**当前状态（2026-11-16）：全链路跑通。** 注入 → AOB 扫描 → hook → 控制台打印密钥，游戏正常运行不崩。

---

## 1. 项目目标

游戏（《蔚蓝档案》PC，IL2CPP 编译）每次大版本更新后 SQLCipher 数据库密钥变化，hook 点地址漂移。本项目：**winmm.dll 代理注入后，自动扫描特征码定位 hook 点，在控制台打印密钥，无需人工 CE 操作。**

最终成果：dll 放进游戏目录，启动游戏即自动输出 `PRAGMA key = "x'...'";`。

---

## 2. 核心技术路线

### 2.1 winmm.dll 代理注入（地基）

`winmm.dll` 是 Windows 早期加载的模块，游戏进程启动即加载。游戏目录放伪造 `winmm.dll`：
- `DllMain` 的 `DLL_PROCESS_ATTACH` 时 `LoadLibraryA` 系统目录真 winmm.dll，所有导出函数指针存 `Orignal*` 全局变量
- `common.asm` 里每个 `Fake*` 导出是 `jmp [Orignal*]` 跳板，逻辑全转发给系统 dll
- `DLL_PROCESS_DETACH` 时 `MF_Uninitialize()` + `FreeLibrary`

### 2.2 特征码扫描定位 hook 点

跨版本验证唯一命中的通配 AOB（见背景文档 §5.1）：

```
31 D2 E8 ?? ?? ?? ?? 48 89 C2 E8 ?? ?? ?? ?? 48 89 C3 48 8B 05 ?? ?? ?? ?? 48 8B 80 B8 00 00 00 48 8B 50 20
```

`winmm.cpp` 的 `PatternScan` / `ParsePattern`：
- 支持 `??` 通配，只扫模块可执行 section（解析 PE 头），首字节快速筛
- 唯一命中才装 hook；多命中告警放弃

`keyScanThread`（DllMain 启动）：每 1s 查 `GetModuleHandleA("GameAssembly.dll")`；宿主校验只在 `BlueArchive.exe` 下工作；命中 → `MF_CreateHookMidFunc` + `MF_EnableHook`。

### 2.3 Mid-function hook（MinHook 源码集成 + mhook_mid 二开）

**架构**：`MiniHook/minhook-1.3.4/` 源码原样编译进项目（保留 `MH_CreateHook` 等函数入口 hook 全部能力），mid-function 能力由 `mhook_mid.{c,h}` 提供，不改 MinHook 源文件，升级可干净 merge。

**hook 执行流**（四段式）：

```
target: E9 rel32 → relay          (5字节 patch + NOP 到指令边界)
relay:  jmp [rip+0] → stub        (14字节, 与 stub 同页)
stub:   pushfq + push 全部16个GPR → MF_CONTEXT 栈上构造
        rcx=&ctx, rbx=ctx基址(callee-saved!), 对齐栈, call handler
        恢复(可含handler修改), popfq, jmp [rip+0] → trampoline
trampoline: MinHook CreateTrampolineFunction 生成
        (被盗指令 rip-relative 自动重定位 + jmp 回 target+stolenLen)
```

**关键设计点**：
- **trampoline 复用 MinHook 的 `CreateTrampolineFunction`**（hde64 反汇编 + rip-relative 重定位），这是当初手写方案 §3.1 坑的根治
- **relay/stub/trampoline 共用一个自分配页**（不用 MinHook 的 `AllocateBuffer`，其 64B slot 装不下 stub），页在目标 ±2GB 内以保证 E9 rel32 可达
- **stub 由 C 代码 emit 裸字节**（每 hook 捕获不同寄存器/调不同 handler，无法静态写死），`MF_CONTEXT` 含全部 16 GPR，handler 可读可改（改后在恢复时生效）
- **patch 时冻结全部线程并修正覆盖区内 RIP**（同 MinHook 原生 `MH_EnableHook` 的 Freeze 机制），见 §3.8
- 独立 `g_mfHooks` 表管理 enable/disable 生命周期，不与 MinHook 的 `g_hooks` 交互

**API**（`mhook_mid.h`）：

```c
typedef struct _MF_CONTEXT { uint64_t Rax, Rcx, Rdx, Rbx, Rsp, Rbp, Rsi, Rdi,
                             R8, R9, R10, R11, R12, R13, R14, R15; } MF_CONTEXT;
typedef void (*MF_HANDLER)(MF_CONTEXT* ctx);

MF_STATUS MF_Initialize(void);
MF_STATUS MF_CreateHookMidFunc(void* pTarget, MF_HANDLER pHandler, void** ppTrampoline);
MF_STATUS MF_EnableHook(void* pTarget);
MF_STATUS MF_DisableHook(void* pTarget);
MF_STATUS MF_Uninitialize(void);
```

业务侧（`winmm.cpp`）只需三行装 hook，handler 是普通 C 函数：

```c
extern "C" void keyHandler(MF_CONTEXT* ctx) {
    uint8_t* s = (uint8_t*)ctx->Rax;          // hook 点处 RAX = hex 密钥串
    int32_t len = *(int32_t*)(s + 0x10);      // IL2CPP String: 长度 +0x10
    ...                                        // 字符 +0x14, printf 输出
}
```

**Hook 点选择**：AOB 命中 +0x0F（`mov rbx,rax` 之前），此时 RAX = 64 字符 hex 密钥串（IL2CPP String）。E9 跳转零寄存器破坏，进 stub 时 RAX 完好。

---

## 3. 开发中踩过的坑（重点！按发现顺序）

> §3.1–3.5 来自第一代手写实现（详见 handoff.md），§3.6–3.8 是 MinHook 二开版踩的。
> 这些坑的共同点：**崩溃点远离出错点**，且全是 x64 机器码/ABI/并发级别的细节。

### 3.1 Capstone/Keystone trampoline 对 rip-relative 指令的 bug（第一代根因）

Capstone 反汇编 rip-relative 指令打印的位移语义与 Keystone 汇编的输入约定不匹配，被搬走的 `mov rax,[rip+...]` 在 trampoline 里指向错误地址。**MinHook 的 hde64+手工重定位方案根治此坑**，这也是引入 MinHook 的核心动机。

### 3.2 trampoline 复制了未被盗的指令

trampoline 多搬了一条 `mov rax,[rax+0xB8]` 且跳回点错误，指令被执行两次 → 解引用 null 崩。**对策**：只搬被盗指令，回跳点必须是原始未动指令的起点。

### 3.3 用 caller-saved 寄存器保存 RSP（第一代）

`mov r10, rsp` 保存栈指针，handler 调用后 r10 已被破坏。**对策**：全局变量保存 RSP（第一代方案）。

### 3.4 校验数组字节对齐写错

通配 disp32 字节"跳过"后仍按连续 memcmp 比对，校验恒失败。**对策**：分段校验。

### 3.5 stub 入口用 movabs 破坏 RAX

`movabs rax, stub; push rax; ret` 进 stub 会覆盖 RAX——而 RAX 正是要捕获的密钥。**对策**：patch 用 E9 rel32（零寄存器影响）。

### 3.6 ctx 基址存在 r8（caller-saved）——MinHook 二开版第一崩

stub 用 `mov r8, rsp` 保存 ctx 基址，`call handler` 后 `mov rsp, r8` 恢复。但 **r8 是 caller-saved**，handler 里的 printf/MessageBoxA 合法地打掉了它 → rsp 装入垃圾 → pop 从野地址读 → 崩。

症状特征：handler 本身执行正常（日志都打印了），返回恢复阶段闪退。这是 §3.3 的同款坑换个寄存器复发——**"用来保存状态的寄存器"自己必须是 callee-saved（rbx/rbp/rsi/rdi/r12-r15）**。CE 实测 r8=野值 vs rsp=栈值坐实。

**对策**：`mov rbx, rsp`（rbx 的原值已提前 push 进 ctx+0x18，恢复序列里的 `pop rbx` 会正确还原）。

### 3.7 lea 的 REX 前缀编码错一位——潜伏 bug

emit `lea r8, [rsp+0x60]` 时写了 `49 8D 44 24 60`。REX=0x49 的 **B 位=1 会把 SIB base 字段（100b）从 rsp 变成 r12**，实际指令是 `lea r8, [r12+0x60]`。正确编码 `4C 8D 44 24 60`（R=1 表示 r8 目的寄存器，B=0 保持 base=rsp）。

后果：ctx->Rsp 字段存进垃圾值（CE 观测 Rsp=0x80 = 当时 r12+0x60），且 ctx->Rax = ctx->R12+0x60 精确相等是铁证。因 Rsp 槽在恢复序列中被 `add rsp,8` 跳过，此 bug 不直接崩，属潜伏污染。

**教训**：手 emit 机器码时 REX 位域（W/R/X/B）必须逐位核对，R/B 不仅作用于 ModRM.reg/rm，还作用于 SIB.base——差一个位寻址就换了寄存器。

### 3.8 打 patch 无线程冻结——最终闪退主根因

`MF_EnableHook` 直接 10 字节非原子写 patch（E9+rel32+NOP）到**正在被执行的热代码**，游戏线程可能执行到写了一半的字节流 → 执行流在 hook 点之前就跑飞 → 后续调用返回垃圾值（RAX 到达 hook 点时是 UnityPlayer 代码指针而非密钥串）→ 闪退。

诊断时最容易误导的一点：症状看起来像"特征码误命中/游戏更新"（RAX 不是密钥），**实际是 patch 竞态破坏了上游执行**。用老版 dll 日志对比（同 AOB 同偏移能拿到密钥）+ CE 确认 trampoline 被盗指令字节正确，才排除误命中定位到竞态。第一代手写版同样没冻结，能跑通纯属时机运气。

**对策**（对齐 MinHook 原生 `MH_EnableHook` 的做法）：
1. `CreateToolhelp32Snapshot` 枚举并 `SuspendThread` 全部其他线程
2. 逐线程取 CONTEXT，RIP 落在 patch 覆盖区间的重定向到 trampoline（enable）/ target（disable）
3. 写 patch + `FlushInstructionCache`
4. 恢复全部线程

另外页面生成的 relay/stub/trampoline 写完后也要 `FlushInstructionCache`。

---

## 4. 文件清单

| 文件 | 角色 |
|---|---|
| `winmm.cpp` | 代理转发 + 扫描线程 + keyHandler（业务 handler） |
| `common.asm` | MASM：winmm 导出 thunk（keyHookStub 已移除，stub 改由框架 emit） |
| `mhook_mid.h/.c` | **mid-function hook 引擎**（MF_* API、stub 生成、relay 页、线程冻结） |
| `MiniHook/minhook-1.3.4/` | MinHook 1.3.4 源码，原样编译（hook.c/trampoline.c/buffer.c/hde64.c） |
| `winmm.def` | 本 dll 导出表（Fake* 名） |
| `handoff.md` | 第一代手写 hook 的历史 handoff（坑位分析仍有效） |
| `6月到8月游戏密钥hook点迁移总结.md` | 跨版本迁移方法论，含特征码与密钥注入机制分析 |

已删除：`ASM_LIB/`（Capstone+Keystone 自研框架，因 §3.1 根本性缺陷弃用）。

---

## 5. 验证流程

1. VS 2019：编译 `winmm-proxy.vcxproj` x64 Release（依赖：MASM 组件；无需 vcpkg）
2. 产物 `x64\Release\winmm.dll` 放游戏目录
3. 启动游戏，预期控制台：
   ```
   [KeyHook] waiting for GameAssembly.dll...
   [KeyHook] AOB unique hit: GameAssembly.dll+0x1BFC38F (VA ...)
   [KeyHook] hook installed, trampoline=...
   [KeyHook] hook installed at GameAssembly.dll+0x1BFC39E, waiting for key...
   [KeyHook] hook fired, RAX=000002xxxxxxxxx      ← 堆地址（IL2CPP String）
   [KeyHook] PRAGMA key = "x'...'";
   ```
   RAX 必须是 `0x2xx...` 堆段地址；若出现 `0x7FF9...` 模块段地址说明执行流被破坏（对照 §3.8）。
4. 密钥拼 `PRAGMA key = "x'...'";` 用 sqlcipher 验证能开 ExcelDB.db
5. 游戏正常进主界面

CE 调试手段（ce-debug 技能）：卡进程后读 hook 点字节、relay/stub/trampoline 反汇编、断点读寄存器。本项目排障大量依赖此手段。

---

## 6. 迁移到其他游戏

### 6.1 保留部分（全部可复用）

- winmm.dll 代理转发机制
- `PatternScan` / `ParsePattern` 扫描器
- **整个 mhook_mid 引擎**（AOB+offset+handler 三要素即可装 hook，无需再手写任何汇编）

### 6.2 需重做的部分

1. **AOB 特征码**：新游戏/新版本重新逆向（背景文档 §3 方法论）
2. **hook 点偏移**：AOB 命中后偏移多少字节（选"目标值已就绪、寄存器不被 patch 跳转破坏"的位置）
3. **handler 逻辑**：读 ctx 哪个字段、按什么内存布局解数据
4. **宿主进程校验**：改 `IsBlueArchiveHost` 的进程名

### 6.3 迁移检查清单

- [ ] 特征码唯一命中
- [ ] hook 点寄存器语义确认（CE 断点实测，别信文档推算）
- [ ] handler 用 __try/__except 包裹读内存
- [ ] 打 patch 走 MF_EnableHook（内含线程冻结，勿绕过）

---

## 7. 密钥归档

测试期间实测拿到的密钥（仅本地解包研究用途，来自自有合法副本的动态分析）：

| 日期 | 密钥（64 hex） | hook RVA | 引擎版本 |
|---|---|---|---|
| 2026-11-16 | `05f508236ff3dc4e5dc7cb6fcea0ede0d90a89753dc5e5d36c30393e9b072f48` | GameAssembly.dll+0x1BFC39E | 第一代手写 |
| 2026-11-16 | 同上（MinHook 二开版复测通过） | 同上 | MinHook 二开 |

> 每次游戏更新密钥会变，重跑一次即得新值。
