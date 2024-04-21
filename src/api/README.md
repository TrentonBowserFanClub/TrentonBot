# API v1

This version of the API uses a local venv to install packages, and only runs locally.

## Creating a venv

Use the following command to create your venv:

```
python3.11 -m venv venv
```

From here, you can source your venv with the following commands.

Windows:

```
.\venv\Scripts\Activate.ps1
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

# API v2

This API allows for interfacing between the webapp and the vehicle code over ROS 2.

# Contributing

## Testing Code

You can test this code locally by running the command:

```
bazel run //src/api:main
```

This will start up the API using uvicorn, running on port 8000 unless the port is in use.

If you want to use args with the app, you can run the following:

```
bazel run //src/api:main -- <ARGS>
```

The extra `--` allows us to skip over the _bazel_ arguments, and start passing arguments directly to our process.

**NOTE**: Right now, certain args such as `--reload` don't work with bazel, as built files are copied into a `bazel-out` directory instead of using the files directly in the repo. This means, when running with `--reload`, uvicorn watches the foles in the cached build directory and NOT the files that you'd be editing in the repo. Looking for a workaround in this.

## Building a Docker Image Locally

The docker image can be built and published locally using the command:

```
bazel run //src/api:tarball
```

This will publish the image to your local docker images based on the provided tag:

```
INFO: Running command line: bazel-bin/src/api/tarball.sh
f4ba9898d2a2: Loading layer  17.56kB/17.56kB
The image trentonbot_api:latest already exists, renaming the old one with ID sha256:ccb7026cdc6bcb8f6a46a86b8e363581f33e5168f5cab5cde88f77cd8e0539b5 to empty string
Loaded image: trentonbot_api:latest
```

In this case, if you want to run the image locally, you can used the loaded image above to run:

```
docker run -p 8000:8000 trentonbot_api:latest
```

This will spin up the docker container locally, and allow you to test from it.
