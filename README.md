# Running benchmarks

## Setup

### Link the SDQL repository

Create a symlink to the SDQL repository:

```sh
# clone the sdql repo somewhere (e.g. home directory)
cd ~ && git clone https://github.com/edin-dal/sdql/
# create a symlink here to the SDQL programs directory
cd - && ln -s ~/sdql/progs progs
```

### Install Python version and virtualenv

Install `pyenv` and `pyenv-virtualenv`:

```sh
curl https://pyenv.run | bash
# follow these instructions to install pyenv-virtualenv
# https://github.com/pyenv/pyenv-virtualenv?tab=readme-ov-file#installing-as-a-pyenv-plugin
```

On macOS:

```sh
brew install pyenv
brew install pyenv-virtualenv
# follow these instructions if you get "Failed to activate virtualenv"
# https://github.com/pyenv/pyenv-virtualenv/issues/387
```

Get the required Python version:

```sh
export VERSION=$(cat pyproject.toml| grep -Poi '^python = "\K(\d+.\d+.\d+)')
```

On macOS run it as `ggrep` after `brew install grep` to use GNU grep:

```sh
export VERSION=$(cat pyproject.toml| ggrep -Poi '^python = "\K(\d+.\d+.\d+)')
```

Create an environment with the required version:

```sh
pyenv install $VERSION
pyenv virtualenv $VERSION wcoj-benchmark
```

Setting it as the local environment will activate it:

```sh
pyenv local wcoj-benchmark
```

### Install Poetry manager and packages

Install the package manager `poetry`:

```sh
curl -sSL https://install.python-poetry.org | python3 -
```

Install the required packages to your environment:

```sh
poetry install
```

## Run

Convert the required datasets from Parquet to CSV, then copy them to `progs/../datasets/job/`.

Results are stored in the `timings` directory. Delete existing `wcoj` results to trigger a re-run.

You will also need to replace `free-join` results with ones generated on your machine.

### JOB queries

Use the datasets and run the code from https://github.com/SIGMOD23p561/free-join.

### LSQB queries

Use the datasets and run the code from https://github.com/remysucre/gj-vs-binary.

### Usage

Now run the benchmarks as follows:

```sh
python __main__.py --benchmark [JOB|LSQB] 
```
