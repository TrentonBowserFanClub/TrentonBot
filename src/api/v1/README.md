# API v1

This version of the API uses a local venv to install packages, and only runs locally.

## Creating a venv

Use the following command to create your venv:

```
python3.11 -m venv venv
```

From here, you can source your venv with the following commands.
Windows, probably:

```
./venv/bin/Activate.ps1
```

Linux:

```
. venv/bin/activate
```

## Installing needed packages

You can install packages required for the application using the command:

```
pip install -r requirements.txt
```

## Saving pip requirements

Run the following command to update the `requirements.txt` file, which is needed whenever you install a new package:

```
pip freeze > requirements.txt
```
