# 📖 Conck - 命令行通讯录管理系统

## 📝 简短介绍

Conck 是一个基于 Java 编写的轻量级命令行通讯录管理系统。项目采用清晰的分层架构设计，将数据模型、业务逻辑、持久化存储与用户交互（UI）完全解耦，具有极高的可维护性和扩展性。

**核心特性：**
- **完整的 CRUD 功能**：支持联系人的查询、添加、修改和删除。
- **多维度检索与排序**：支持按姓名或号码进行模糊搜索，并支持按姓名或号码进行排序展示。
- **本地文件持久化**：联系人数据会自动保存在本地 `contacts.txt` 文件中，关闭程序后数据不会丢失。
- **现代化 Java 语法**：使用了 Java 14+ 的 `record` 关键字定义数据模型，并利用 Java Stream API 处理集合数据，代码简洁高效。

**技术栈：** Java (JDK 16+) / 面向接口编程 / Stream API / 文件 I/O

---

## 🚀 如何使用

如果你已经拿到了编译好的可执行文件（或正在 IDE 中运行），请按照以下步骤操作：

1. **启动程序**：运行 `Main` 类或执行对应的 `jar` 包。
2. **主菜单操作**：启动后你将看到主菜单，输入以下指令（不区分大小写）执行相应操作：
   - `look up`：进入检索菜单（可排序、搜索、查看所有联系人）。
   - `add`：进入添加页面（根据提示依次输入姓名和号码）。
   - `modify`：进入修改页面（搜索 -> 选择目标 -> 输入要修改的字段 -> 输入新值）。
   - `delete`：进入删除页面（搜索 -> 选择目标 -> 输入 `yes` 确认删除）。
   - `quit`：保存并退出程序。
3. **返回上一级**：在任何子菜单中，输入 `return` 即可退回主菜单。

**💡 使用示例：**
```text
> add
return --back to last page
<any name> --record name
<any number> --record number
张三
13800138000
Successful operation
```
*(注：数据会自动保存在项目根目录下的 `contacts.txt` 中)*

---

## 🛠️ 如何自行编译后使用

本项目未使用 Maven 或 Gradle 等构建工具，你可以直接使用 JDK 自带的命令进行编译和运行。

**环境要求：**
- 必须安装 **JDK 16 或更高版本**（因为代码中使用了 `record` 关键字和文本块 `"""` 特性）。

**编译与运行步骤：**

1. **克隆或下载本项目**到本地。
2. 打开终端（命令行），进入项目所在的根目录（即 `src` 文件夹所在的目录）。
3. **编译代码**：
   使用 `javac` 命令将源代码编译到 `bin` 目录中（确保你已经创建了 `bin` 文件夹，或者让 `-d` 参数自动创建）：
   ```bash
   # 在 Windows 或 Linux/Mac 下执行：
   javac -d bin src/conck/model/*.java src/conck/repository/*.java src/conck/service/*.java src/conck/ui/*.java src/conck/Main.java
   ```
4. **运行程序**：
   使用 `java` 命令运行编译好的主类（指定 `classpath` 为 `bin` 目录）：
   ```bash
   java -cp bin conck.Main
   ```
   
此时，程序就会在终端中启动，你可以开始体验你的通讯录系统了！

---

## 📂 项目结构说明

```text
Conck/
├── src/
│   ├── conck/
│   │   ├── model/        # 数据模型（Contact.java）
│   │   ├── repository/   # 数据持久化层（接口 + 文件实现）
│   │   ├── service/      # 业务逻辑层（排序、搜索、增删改查）
│   │   ├── ui/           # 控制台交互层（ConsoleUI.java）
│   │   └── Main.java     # 程序入口
├── bin/                  # 编译后的字节码输出目录（Git 忽略）
└── contacts.txt          # 本地数据文件（首次添加联系人时自动生成）
```