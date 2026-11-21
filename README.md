<p align="center">
  <img src="assets/github/images/banner-yatrices.png" alt="Yatrices: linear algebra in C++20 with a Python front end" width="100%">
</p>

<p align="center">
  <a href="https://github.com/yassinox12834/yatrices/releases/latest"><img alt="latest release" src="https://img.shields.io/github/v/release/yassinox12834/yatrices?color=0aa5a5"></a>
  <img alt="C++20" src="https://img.shields.io/badge/C%2B%2B-20-0b6e8a">
  <img alt="Python 3.10+" src="https://img.shields.io/badge/Python-3.10%2B-14a86b">
  <img alt="status" src="https://img.shields.io/badge/status-early%20days-lightgrey">
</p>

# Yatrices

Yatrices is a small linear algebra library. The heavy lifting happens in a C++ core called **YatricesEngine**, and **yatricespy** lets you use it from Python. Think of it as a homemade little cousin of `numpy.linalg`.

The name is just **Ya**ssine + ma**trices**. Nothing cleverer than that, I'm afraid.

## Why I'm doing this

For months I called `np.linalg.solve`, got my answer and moved on, without the faintest idea how it got there. That started to bother me. I'd like to work in machine learning one day, and ML is linear algebra almost wherever you look, so I decided to build it myself: in C++ first, then made usable from Python.

So it's a learning project above all. I'd much rather understand a hundred lines properly than copy a thousand.

<img src="assets/github/images/logo.png" alt="Yatrices logo: a spotted eagle ray" width="110" align="right">

**About the logo:** it's a spotted eagle ray. Rays glide along with hardly any effort, and that's how I'd like the library to feel. Operations should chain together naturally (`x + y + z`), and nothing should be heavier than it needs to be: a `Vec` is 16 bytes plus its data, with nothing hidden.

## Getting started

> **Coming later:** I'll publish yatricespy on [PyPI](https://pypi.org/) (so `pip install yatricespy` just works) and YatricesEngine on [vcpkg](https://vcpkg.io/). Until then, you build both from this folder as shown below.

For the version number, rely on the [latest GitHub release](https://github.com/yassinox12834/yatrices/releases/latest). It is the reference; the badge above reads from it, and the files in this folder may be ahead of the last release.

You'll need CMake 3.20 or later, a C++20 compiler (GCC 10, Clang 15 or Visual Studio 2019 16.10, or anything newer) and, for the Python side, Python 3.10 or later.

**Python** (pip compiles the engine for you, so a C++ compiler must be on your PATH; on Windows, MSYS2's g++ works):

```bash
python -m pip install ".[test]"
```

```python
import yatricespy as yt

x = yt.Vec([1, 2, 3])
y = yt.Vec([4, 5, 6])
print(x + y)           # [5, 7, 9]
print(x @ y)           # 32.0
print(yt.cross(x, y))  # [-3, 6, -3]

A = yt.Matrix([[1, 2], [3, 4]])
print(A.T)             # [[1, 3],
                       #  [2, 4]]
print(A.trace())       # 5.0
```

Importing it as `yt` is wholeheartedly recommended: write `import yatricespy as yt`. It's the same kind of acronym as `import numpy as np`, short to type and instantly recognisable, and every example in this repository uses it.

**C++:**

```bash
cmake -S . -B build
cmake --build build
./build/cpp/examples/example_vector_ops
```

Each side has its own guide with the full list of what's available: [Python](python/README.md) and [C++](cpp/README.md). If you want to run or write tests, have a look at [TESTING.md](TESTING.md).

## Where things stand

<p align="center"><img src="assets/github/images/banner-wip.png" alt="Work in progress, even the bits marked done" width="100%"></p>

The vector type is done, in both languages: `Vec` (double) and `Vecf` (float), with arithmetic, `dot`, `cross`, norms, sums and means, `angle`, `project`, `allclose`, and error messages that actually tell you what went wrong.

The `Matrix` type arrived in the C++ engine in 0.0.2 and in Python in 0.0.4: `Matrix` (double) and `Matrixf` (float), with arithmetic, transpose, trace, diagonals, row operations, reductions and broadcasting, checked by real C++ tests and by Python tests against NumPy. But even the "finished" bits have plenty left to do:

- there's no matrix-vector or matrix-matrix product (that's next, see the roadmap)
- the newer `Vec` functions (`argmin`, `argmax`, `normalise`, `==`, ...) aren't in Python yet either
- you can't convert to or from NumPy arrays without going through a list
- there's no element-wise `abs`, `sqrt`, `exp` or division, and no slicing or views

There are no tutorials yet either. The plan is a short one for every algorithm, explaining what it does and why it works, and they'll go in [`docs/`](docs/) later on.

What's coming next is listed in [ROADMAP.md](ROADMAP.md), and everything that has changed is in [CHANGELOG.md](CHANGELOG.md).

## How it's organised

```
cpp/      YatricesEngine, the C++ core (knows nothing about Python)
python/   yatricespy, the Python package
docs/     one tutorial per concept (none written yet)
after/    stubs for everything planned after 0.0.2
assets/   logo and banners
```

## Fancy helping?

You're very welcome, whatever your level. If you're learning this stuff too, even better.

- **Spotted a bug, or something unclear?** Open an issue. "I didn't follow this bit of the README" counts as a perfectly good issue.
- **Got an idea?** Open an issue first and we can talk it through.
- **Sending code?** Please stick to C++20, follow `.clang-format`, and check anything numerical against NumPy.

One small thing: since I'm using this project to learn, I might occasionally ask to write a feature myself, even if you've already solved it. Please don't take it personally. Reviews and "have you thought about…?" comments are always appreciated.

<p align="center"><img src="assets/github/images/banner-star.png" alt="Found this useful? A star would be lovely" width="100%"></p>

## Licence

Yatrices is released under the MIT licence. See [LICENSE](LICENSE).

---

<p align="center">Made in Casablanca, with patience, a good few late evenings and far too many <code>static_assert</code>s. 🌊</p>
