---
name: cpp-dev-standards
description: C++ 编码规范。编写、审查或重构 C++ 代码时，强制遵循命名约定、代码风格、项目结构和最佳实践。
---

# C++ 编码规范

本文档定义了 C++ 项目的编码规范和最佳实践，覆盖 C++17/20/23。**所有团队成员在编写、审查和重构 C++ 代码时，必须遵循以下规范。**

---

## 通用原则

1. **可读性优先** — 代码是写给人读的，顺便给机器执行
2. **一致性至上** — 比"哪种风格更好"更重要的是"整个代码库风格统一"
3. **RAII 为首** — 资源管理全部交给 RAII，杜绝手动 new/delete
4. **const 优先** — 默认 immutable，需要时才 mutable
5. **最小作用域** — 变量声明在离使用最近的位置，作用域越小越好
6. **防御式编程** — 永远假设输入可能非法

---

## 命名规范

| 类别             | 风格                           | 示例                                |
| -------------- | ---------------------------- | --------------------------------- |
| 命名空间           | `snake_case`                 | `namespace my_project`            |
| 类 / 结构体        | `PascalCase`                 | `class TcpConnection`             |
| 函数 / 方法        | `snake_case`                 | `void send_message()`             |
| 变量（局部）         | `snake_case`                 | `int retry_count`                 |
| 成员变量           | `snake_case_`（尾下划线）          | `std::string name_`               |
| 常量 / constexpr | `kPascalCase` 或 `snake_case` | `const int kMaxRetries`           |
| 宏              | `UPPER_SNAKE_CASE`           | `#define VERSION_MAJOR 1`         |
| 枚举类枚举值         | `snake_case`                 | `enum class Color { red, green }` |
| 文件             | `snake_case`                 | `tcp_connection.h`                |

```cpp
// ✓ GOOD
namespace payment_service {

constexpr int kMaxRetryCount = 3;

class OrderManager {
public:
    void process_order(int order_id);

private:
    std::vector<Order> pending_orders_;
};

}  // namespace payment_service
```

```cpp
// ✗ BAD
namespace PaymentService {
class order_manager {
    void ProcessOrder(int orderId);
    vector<Order> pendingOrders;
};
}
```

---

## 文件组织

### 头文件 (.h)

```cpp
// tcp_connection.h
#pragma once  // 或使用 #ifndef 传统 guard

#include <memory>
#include <string>
#include <string_view>

// 禁止 using namespace 在全局作用域
namespace network {

class TcpConnection {
public:
    explicit TcpConnection(std::string_view host, int port);
    ~TcpConnection() = default;

    // 禁止拷贝，允许移动
    TcpConnection(const TcpConnection&) = delete;
    TcpConnection& operator=(const TcpConnection&) = delete;
    TcpConnection(TcpConnection&&) noexcept = default;
    TcpConnection& operator=(TcpConnection&&) noexcept = default;

    void connect();
    void send(std::string_view data);
    bool is_connected() const;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

}  // namespace network
```

### 源文件 (.cpp)

```cpp
// tcp_connection.cpp
#include "tcp_connection.h"

// 内部辅助函数放在匿名命名空间或 .cpp 文件静态函数中
namespace {

bool is_valid_port(int port) noexcept {
    return port > 0 && port <= 65535;
}

}  // anonymous namespace

namespace network {

TcpConnection::TcpConnection(std::string_view host, int port)
    : pimpl_(std::make_unique<Impl>()) {
    // ...
}

void TcpConnection::connect() {
    // ...
}

}  // namespace network
```

### 目录结构约定

```
project/
├── src/                    # 源代码
│   ├── core/               # 核心模块
│   ├── network/            # 网络模块
│   └── utils/              # 工具函数
├── include/                # 公开头文件（或放在 src 同目录）
├── tests/                  # 单元测试
├── third_party/            # 第三方库
└── cmake/                  # CMake 模块
```

---

## 代码风格

### 缩进与格式

- 缩进：**4 个空格**（不使用 Tab）
- 行宽：**120 字符**上限
- 大括号风格：**K&R / 1TBS 风格**（不换行），左大括号放在行尾

```cpp
// K&R 风格（不换行）
bool func(int arg) {
    if (arg > 0) {
        do_something();
    } else {
        do_other();
    }
}
```

> **注：** 如果团队统一使用 clang-format，可在 `.clang-format` 中定义格式规则，实际风格以格式化工具为准。

### 指针与引用

```cpp
// ✓ GOOD
int* ptr;           // 星号靠类型
int& ref;           // 引用符号靠类型
const int& cref;    // const & 靠类型

// 智能指针
auto widget = std::make_unique<Widget>();
auto cache  = std::make_shared<Cache>();

// 原始指针表示非拥有观察者
void render(const Widget* widget);
```

### 初始化

```cpp
// 优先使用 {} 初始化
int count{0};
std::vector<int> values{1, 2, 3};
auto config = Config{};
```

### 类型别名

```cpp
// 使用 using 而非 typedef
using StringList = std::vector<std::string>;
// ✗ typedef std::vector<std::string> StringList;
```

---

## 现代 C++ 使用规范

### 范围与类型推导

```cpp
auto x = compute_value();                    // 值类型
const auto& ref = get_reference();           // const 引用
auto* ptr = dynamic_cast<Base*>(obj);        // 指针

// 使用范围 for 循环
for (const auto& item : container) { /* ... */ }

// 结构化绑定
auto [name, age] = get_person_info();
```

### 智能指针

```cpp
// 独有所有权：unique_ptr
auto resource = std::make_unique<Resource>(args...);

// 共享所有权：shared_ptr（尽量少用，优先 unique_ptr）
auto shared = std::make_shared<Cache>(1024);

// 弱引用：weak_ptr（用于打破循环引用）
std::weak_ptr<Cache> weak = shared;
```

### Lambda 表达式

```cpp
// 指定捕获，避免捕获整个作用域
auto callback = [this, /* explicit captures */](int value) -> bool {
    return value > threshold_;
};

// 通用 lambda（C++14）
auto comparator = [](const auto& a, const auto& b) { return a < b; };
```

### 字符串

```cpp
// 拥有所有权时使用 std::string
std::string name;

// 只读观察时使用 std::string_view（注意生命周期）
void process_name(std::string_view name);
```

---

## 错误处理

### 异常

```cpp
// 使用自定义异常类型层次
class AppError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class ValidationError : public AppError { /* ... */ };
class NetworkError : public AppError { /* ... */ };

// 按值抛出，按引用捕获
void process() {
    try {
        do_something();
    } catch (const ValidationError& e) {
        handle_validation(e);
    } catch (const AppError& e) {
        handle_app_error(e);
    }
}
```

### noexcept

```cpp
// 移动操作和 swap 必须 noexcept
void swap(Type& other) noexcept;

// 不会抛出的函数标注 noexcept
int get_value() const noexcept;
```

### 错误码

对于性能关键的路径或嵌入环境，优先使用 `std::expected`（C++23）或 `tl::expected`：

```cpp
// 使用 expected 风格替代异常（可选）
enum class Error { kNotFound, kInvalidInput, kTimeout };

using Result = std::expected<Value, Error>;

Result parse(std::string_view input) noexcept;
```

---

## 并发

### 线程安全

```cpp
class ThreadSafeCounter {
public:
    int increment() noexcept {
        std::lock_guard<std::mutex> lock(mutex_);  // RAII 加锁，必须命名
        return ++count_;
    }

    int value() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }

private:
    mutable std::mutex mutex_;
    int count_{0};
};
```

### 多锁操作

```cpp
// 使用 scoped_lock 同时锁定多个互斥体（避免死锁）
void transfer(Account& from, Account& to, double amount) {
    std::scoped_lock lock(from.mutex_, to.mutex_);
    from.balance_ -= amount;
    to.balance_ += amount;
}
```

---

## 模板与泛型

```cpp
// C++20: 使用 concept 约束模板参数
template<std::integral T>
T gcd(T a, T b) noexcept {
    while (b != 0) {
        a = std::exchange(b, a % b);
    }
    return a;
}

// 自定义 concept
template<typename T>
concept Serializable = requires(const T& t) {
    { t.serialize() } -> std::convertible_to<std::string>;
};

template<Serializable T>
void save_to_file(const T& obj, std::string_view path);
```

---

## 注释规范

### Doxygen 风格

```cpp
/// @brief 连接到远程服务器
/// @param host 主机名或 IP 地址
/// @param port 端口号（1-65535）
/// @return 连接是否成功
/// @throws NetworkError 连接超时或拒绝时抛出
bool connect(std::string_view host, int port);
```

### 代码注释原则

- 写 **Why**（为什么这样写），而非 **What**（这段代码在做什么）
- TODO 注释注明责任人：`// TODO(xiaoming): 需要优化 O(n²) 算法`
- 不要注释不言自明的代码

```cpp
// ✗ BAD: 注释显而易见的逻辑
// 如果 x 大于 0，返回 true
if (x > 0) return true;

// ✓ GOOD: 解释非直觉的设计决策
// 这里使用轮询而非事件回调，因为目标系统不支持中断
while (!device_ready()) { /* spin */ }
```

---

## C++23 新特性

如果项目使用 C++23 标准，以下新特性应优先于旧写法使用：

### `std::expected` — 带错误码的返回值（替代异常或输出参数）

```cpp
#include <expected>

enum class ParseError { kInvalidInput, kOverflow, kTrailingChars };

// C++23: 函数式错误处理，无需异常机制
using ParseResult = std::expected<int, ParseError>;

ParseResult parse_int(std::string_view s) noexcept {
    int value{0};
    for (char c : s) {
        if (c < '0' || c > '9') return std::unexpected(ParseError::kInvalidInput);
        int next = value * 10 + (c - '0');
        if (next < value) return std::unexpected(ParseError::kOverflow);
        value = next;
    }
    return value;
}

// 调用方可以链式处理
void process(const std::string& input) {
    auto result = parse_int(input)
        .and_then([](int v) -> ParseResult {
            return (v >= 0) ? ParseResult(v) : std::unexpected(ParseError::kInvalidInput);
        })
        .or_else([](ParseError e) -> ParseResult {
            log_error("parse failed: {}", static_cast<int>(e));
            return ParseResult(0);  // fallback
        });
}
```

### `std::print` / `std::println` — 类型安全的格式化输出

```cpp
#include <print>

// 替代 std::cout / printf / fmt::print
std::println("Hello, {}!", name);
std::println("value = {}, count = {}", 3.14, 42);

// 写入文件
auto file = std::fopen("log.txt", "a");
std::print(file, "error: {}\n", error_msg);
std::fclose(file);
```

### `std::mdspan` — 多维数组视图（零开销抽象）

```cpp
#include <mdspan>
#include <vector>

// 将一维数据视为二维矩阵（不复制数据，无运行时开销）
std::vector<float> data = {1, 2, 3, 4, 5, 6};
std::mdspan matrix(data.data(), 2, 3);  // 2 行 3 列

// 访问元素
float center = matrix[1, 1];  // matrix[1][1] 的现代写法

// 子视图
auto row0 = std::submdspan(matrix, 0, std::full_extent);
```

### `std::flat_map` / `std::flat_set` — 排序连续容器

```cpp
#include <flat_map>
#include <flat_set>

// 替代 std::map/set，内存连续，缓存友好，小数据量性能更好
std::flat_map<int, std::string> config = {
    {1, "enabled"},
    {2, "timeout"},
};

// 查找接口与 std::map 一致
if (auto it = config.find(1); it != config.end()) {
    std::println("found: {}", it->second);
}

// flat_set 同理
std::flat_set<int> ids = {3, 1, 4, 1, 5};
```

### `std::optional` 的 Monadic 操作

```cpp
#include <optional>

std::optional<int> parse(const std::string& s);

// C++23: 链式调用，避免嵌套 if
auto result = parse("42")
    .and_then([](int v) -> std::optional<int> {
        return (v > 0) ? std::optional(v * 2) : std::nullopt;
    })
    .transform([](int v) { return std::to_string(v); })
    .or_else([] { return std::optional<std::string>("fallback"); });

// 等价于旧写法:
// auto opt = parse("42");
// if (opt && *opt > 0) { auto s = std::to_string(*opt * 2); ... }
```

### `std::views::enumerate` — 带索引的范围遍历

```cpp
#include <ranges>

// C++23: 遍历时同时获取索引和值
for (auto [index, value] : data | std::views::enumerate) {
    std::println("[{}] = {}", index, value);
}

// 旧方式需要手动维护索引:
// for (size_t i = 0; i < data.size(); ++i) { ... }
```

### `std::ranges::to` — 范围直接转换为容器

```cpp
#include <ranges>

// 将一个 range 直接转换为任意容器
auto result = data
    | std::views::filter([](int x) { return x > 0; })
    | std::views::transform([](int x) { return x * x; })
    | std::ranges::to<std::vector>();

// 无需再写 std::vector<int> v(begin, end);
```

### `deducing this` — 推导 this 参数

```cpp
struct Visitor {
    // C++23: 一个函数同时处理左值和右值 this
    void visit(this auto&& self, auto&& callback) {
        callback(std::forward_like<decltype(self)>(self.value_));
    }

private:
    std::string value_;
};
```

### 模块（Modules）— 替代头文件

```cpp
// math.cppm — C++20/23 模块接口文件
export module math;

export int add(int a, int b) {
    return a + b;
}

// main.cpp — 使用模块
import math;

int main() {
    return add(1, 2);
}
```

> 模块需要编译器支持（GCC 14+, Clang 17+, MSVC 2022 17.5+）且 CMake 配置较复杂，建议逐步采用。

---

## CMake 构建规范

```cmake
cmake_minimum_required(VERSION 3.20)
project(my_project VERSION 1.0.0 LANGUAGES CXX)

# 标准版本（按需切换 C++20 或 C++23）
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()
```

---

## 禁止事项（Code Review 红线）

- ❌ 裸 `new` / `delete`（使用智能指针或 RAII）
- ❌ C 风格强制转型（使用 `static_cast` / `dynamic_cast` 等）
- ❌ 全局 `using namespace std;`
- ❌ 头文件中 `using namespace`
- ❌ 未初始化的局部变量
- ❌ 魔数（magic number）——必须定义为命名常量
- ❌ 将 `0` 或 `NULL` 作为空指针（使用 `nullptr`）
- ❌ 非 const 全局变量
- ❌ 在构造函数/析构函数中调用虚函数
- ❌ 不加锁访问共享可变数据
- ❌ `std::endl`（使用 `'\n'` 替代）
- ❌ 不做边界检查的 `memcpy` / `sprintf` 等 C 函数

---

## 审查清单（Checklist）

在提交 C++ 代码前逐项检查：

- [ ] 所有变量已初始化（`{}` 初始化）
- [ ] 所有指针使用智能指针或明确为非拥有
- [ ] 所有单参数构造函数加了 `explicit`
- [ ] Rule of Zero / Five 已正确处理
- [ ] 需要 `const` / `constexpr` 的地方都已标注
- [ ] 需要 `noexcept` 的函数已标注
- [ ] 模板参数已用 concept 约束（C++20）
- [ ] C++23 特性优先使用（`std::expected` / `std::print` / `std::flat_map` 等）
- [ ] 没有裸 `new`/`delete`/`malloc`/`free`
- [ ] 没有 C 风格转型
- [ ] 没有 `using namespace std;` 在头文件全局作用域
- [ ] 头文件自包含（自足）
- [ ] 错误处理策略一致（异常 / expected / 错误码）
- [ ] 锁使用 RAII（`lock_guard`/`scoped_lock`）
- [ ] 多线程访问有保护

---

## 自定义区

> 💡 **本规范为模板**，请根据团队实际情况补充：
>
> - 本团队使用的 C++ 标准：__________ (C++17 / C++20 / **C++23**)
> - C++23 特性启用清单（如 `std::print`、`std::expected`、`std::flat_map`、Modules 等）：__________
> - 禁止使用的第三方库：__________
> - 强制使用的第三方库：__________
> - 代码格式化工具及配置（clang-format / Artistic Style）：__________
> - 静态分析工具（clang-tidy / PVS-Studio 等）：__________
> - 额外红线规则：__________
