# Contributing Guide

## Code Style Guidelines

### Naming conventions 

The following conventions were chosen to be compatible with the Qt coding style
- Variables use camelCase
- Members use prefix m_ + camelCase
- Classes use PascalCase
- Functions use camelCase
- Constants use PascalCase
- Namespaces are nested and use snake_case

For more information refer to `.clang-tidy`

### Formatting

- Use 4 spaces for indentation
- Maximum line length: 100
- Braces style K&R

For more information refer to `.clang-format`

### File Naming and structure

- One class per file 
- File name should match the class name
- Source files use snake_case
- Header/source pairs should share the same name
- Use `.hpp` for headers and `.cpp` for sources

#### Folder structure 

```
PhoneNumberRecognition/
├── include/
│   └── phone_number/
│       └── api.hpp
│
├── src/
│   └── phone_number/
│      ├── api.cpp
│
├── tests/
├── resources/
├── CMakeLists.txt
```

## Branch Naming

All branches follow the pattern `type/description`:

| Type      | Purpose                          | Example                      |
|-----------|----------------------------------|------------------------------|
| `feature` | New functionality                | `feature/user-auth`          |
| `bugfix`  | Fixing a bug                     | `bugfix/login-null-pointer`  |
| `chore`   | Maintenance, refactoring, config | `chore/update-dependencies`  |

Rules:
- Use lowercase and hyphens: `feature/user-auth` not `Feature/UserAuth`.
- Should be short but descriptive enough.

---

## Commit Message format

```
type(scope): description

[optional body]
```

### Types

| Type       | When to use                                     |
|------------|--------------------------------------------------|
| `feat`     | Adding new functionality                         |
| `fix`      | Fixing a bug                                     |
| `refactor` | Code change that neither fixes a bug nor adds a feature |
| `docs`     | Documentation only                               |
| `test`     | Adding or updating tests                         |
| `chore`    | Build, CI, dependencies, config                  |
| `style`    | Formatting, whitespace (no logic change)         |
| `perf`     | Performance improvement                          |
| `ci`       | CI/CD pipeline changes                           |

### Scope

The scope is optional but recommended. 

### Example

```
feat(vcard-parser): add serialziation
```

### Rules

- Imperative speech: "add feature" not "added feature".
- Lowercase, no period: `feat: add login` not `feat: Add login.`
- Keep the commit name as short as possible


## Hooks setup

To enable hooks use 

``` 
chmod +x .githooks/pre-commit
chmod +x .githooks/pre-push
chmod +x .githooks/commit-msg
git config core.hooksPath .githooks
```
Requires bash and clang-tidy installed. Assumes compile_commands.json is in build/ adjust if needed

## Pull Requests

- Ensure all tests pass
- Run clang-format before committing
- Run clang-tidy checks
- Keep pull requests focused and small
---
