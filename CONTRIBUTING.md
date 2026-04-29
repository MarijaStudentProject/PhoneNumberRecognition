# Contributing Guide

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

---
