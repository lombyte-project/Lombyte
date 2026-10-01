# Commit message standard

Every commit subject uses one shape:

```
<type>: <summary>
```

The `<type>` names the change's primary area. The subject may name a module or
symbol, but never repeats an area prefix: `decomp: textbin: promote …` is
wrong; write `decomp: promote …`.

## Types

| Type     | Use for                                                                   |
| :------- | :------------------------------------------------------------------------ |
| `decomp` | Exact-C promotions, source work, source refactors, oracle/source plumbing |
| `overlay` | The level overlay layer: its catalogue, `src/overlays/` stubs and promoted C, the overlays build, its map and report plumbing |
| `docs`   | README and documentation, source headers, ROLE comments, reference data   |
| `chore`  | Non-behavioral cleanup, generated artifacts, tooling, data refreshes      |
| `fix`    | A bug fix in tooling or the build                                         |
| `config` | Ownership/config data (`rnc1.us.yaml`, categories, linker bindings)       |

A commit that promotes sources and also refreshes the map is still `decomp:`;
the map is a derived artifact. Prefer `decomp:` whenever the commit changes
`src/` behavior. Work on the level overlays is `overlay:` whatever it touches
(`config/overlays/`, `src/overlays/`, the overlay parts of the scripts and
the map); `decomp:` stays the executable's.

## Rules

- Lowercase type, exactly one `: ` separator, imperative summary, no trailing
  period, subject length ≤ 120 characters.
- Do not repeat the type or add a scope token (for example `textbin:`) in the
  summary.
- One commit, one primary area; split unrelated areas instead of stacking
  prefixes.
- Bodies are free-form and optional; put detail (metrics, notes) there.
- No agent trailers (`Co-Authored-By:`, `Claude-Session:`) and no links to AI
  chat or agent sessions, in commit messages, pull request descriptions, or
  review comments. Sessions are private; this repository is public. The hook
  drops the trailers and rejects any remaining session link.
- Author and committer are the maintainer, with the same name and e-mail as
  the existing history (`git log -1 --format='%an <%ae>' main`), including
  for commits prepared by an AI agent. Never commit under an agent identity
  such as `Claude <noreply@anthropic.com>`; the hook rejects anthropic.com
  and openai.com addresses.

## Examples

```
decomp: promote fun_002212b8 (424 B)
decomp: promote run-11 owners (20 units)
overlay: promote 159 shared functions into src/overlays
docs: add recovered engine source layout reference
chore: refresh treemap
config: refresh recovered-name unit paths for promoted units
fix: make verify-baseline.sh executable
```

## Enforcement

The repository version-controls a `commit-msg` hook at `scripts/commit-msg.py`.
Enable it once per checkout:

```sh
ln -sf ../../scripts/commit-msg.py .git/hooks/commit-msg
```

The hook rejects any subject that does not match the standard above.
