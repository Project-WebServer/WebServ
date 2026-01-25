# Conventions

Minimal workflow rules to keep the project clean and professional when using GitHub Free plan.

---

## Branching rules

Never work directly on `main`.

Create one branch per task:
- `feature/<description>`
- `fix/<description>`
- `docs/<description>`
- `refactor/<description>`

Examples:
- `feature/http-parser`
- `fix/socket-leak`
- `docs/update-readme`

---

## Basic workflow

Start a task:
```bash
git checkout main          # switch to main branch
git pull origin main       # update local main
git checkout -b feature/x  # create and enter new branch
```

Before opening PR:
```bash
git fetch origin              # update remote references
git rebase origin/main        # reapply commits on top of latest main
git push -u origin feature/x  # push branch to GitHub
```

---

## Commit message style

Use clear, consistent messages:
- `feat: add request parser`
- `fix: handle empty headers`
- `refactor: simplify event loop`
- `docs: update architecture`
- `test: add parser tests`

---

## Branch cleanup

After merge, delete the branch locally:
```bash
git branch -d feature/x        # delete locally
```

---

## Git command reference

### Branch management
```bash
git branch                  # list local branches
git branch -a               # list local and remote branches
git checkout -b feature/x   # create and switch to new branch
git switch -c feature/x     # same as above (modern alternative)
git checkout main           # switch to main branch
git switch main             # same as above
git branch -d feature/x     # delete local branch (safe)
git branch -D feature/x     # force delete local branch
```

## Sync with remote
```bash
git fetch origin            # update remote references
git pull origin main        # fetch + merge main into current branch
git rebase origin/main      # rebase current branch on top of main
git push origin feature/x   # push branch to GitHub
git push                   # push current branch (if upstream is set)
```

## Inspect changes
```bash
git status                  # show current state
git diff                    # show unstaged changes
git diff --staged           # show staged changes
git log --oneline --graph --all  # visualize commit history
```

## Undo and fix
```bash
git restore file.cpp        # discard unstaged changes in file
git restore --staged file.cpp # unstage file
git reset --soft HEAD~1     # undo last commit, keep changes
git reset --hard HEAD~1     # undo last commit and discard changes (dangerous)
git revert <commit-hash>    # safely revert a commit in shared history
```

## Temporary storage
```bash
git stash                  # save changes temporarily
git stash pop              # restore stashed changes
```