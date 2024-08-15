# Python 3.12 Installation on Ubuntu

This guide walks you through the steps to install Python 3.12 on Ubuntu, set it as the default version, remove Python 3.10, and ensure that all required development headers and tools are available.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Step 1: Add the Deadsnakes PPA](#step-1-add-the-deadsnakes-ppa)
- [Step 2: Install Python 3.12](#step-2-install-python-312)
- [Step 3: Set Python 3.12 as the Default Version](#step-3-set-python-312-as-the-default-version)
- [Step 4: Remove Python 3.10 (Optional)](#step-4-remove-python-310-optional)
- [Step 5: Verify the Installation](#step-5-verify-the-installation)
- [Step 6: Update `pip` for Python 3.12](#step-6-update-pip-for-python-312)
- [Conclusion](#conclusion)

## Prerequisites

- Ubuntu (20.04, 22.04, or any other recent version)
- Sudo privileges

## Step 1: Add the Deadsnakes PPA

The Deadsnakes PPA contains newer versions of Python that are not available in the default Ubuntu repositories.

1. Open a terminal.
2. Add the Deadsnakes PPA:

   ```bash
   sudo add-apt-repository ppa:deadsnakes/ppa
   sudo apt-get update
   ```

## Step 2: Install Python 3.12

Install Python 3.12 along with its development headers and tools:

```bash
sudo apt-get install python3.12 python3.12-venv python3.12-dev python3.12-distutils
```

- `python3.12`: Installs the Python 3.12 interpreter.
- `python3.12-venv`: Installs the virtual environment package for Python 3.12.
- `python3.12-dev`: Installs the development headers, necessary for compiling C extensions.
- `python3.12-distutils`: Installs `distutils` for building and installing Python modules.

## Step 3: Set Python 3.12 as the Default Version

Use the `update-alternatives` tool to set Python 3.12 as the default for `python3`:

```bash
sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.12 1
sudo update-alternatives --config python3
```

To also set `python` (without the `3` suffix) to point to Python 3.12:

```bash
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.12 1
sudo update-alternatives --config python
```

Follow the on-screen instructions to select Python 3.12 as the default.

## Step 4: Remove Python 3.10 (Optional)

If you no longer need Python 3.10, you can remove it:

```bash
sudo apt-get remove python python-dev
sudo apt-get autoremove
```

This will remove Python 3.10 and any packages that were automatically installed with it and are no longer required.

## Step 5: Verify the Installation

Ensure that Python 3.12 is the default:

```bash
python --version
python3 --version
```

Both commands should return `Python 3.12.x`.

## Step 6: Update `pip` for Python 3.12

Ensure `pip` is updated for Python 3.12:

```bash
python3.12 -m ensurepip --upgrade
python3.12 -m pip install --upgrade pip
```

You can verify `pip` with:

```bash
python3.12 -m pip --version
```

## Conclusion

You have successfully installed Python 3.12 on Ubuntu, set it as the default version, and removed Python 3.10. Your environment is now ready for development with the latest Python version, including support for virtual environments and compiling C extensions.

If you encounter any issues or need further customization, refer to the official Python documentation or the Deadsnakes PPA documentation.

--- 

This README provides all the necessary steps for installing and configuring Python 3.12 on Ubuntu. Feel free to adapt it further to fit specific project requirements or additional installation needs.