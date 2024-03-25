import uvicorn
import sys

if __name__ == "__main__":
    # We insert this after the default argv at position 0
    # to ensure we bind to the `FastAPI` variable saved in
    # our app. We put this in here as this is the root uvicorn
    # invocation - otherwise we'd need to put it in all py_binary
    # and oci_image rules.
    #sys.argv.insert(1, "src.api.v1.api:app")
    sys.exit(uvicorn.main())

    # sys.exit(uvicorn.run("src.api.v1.api:app", host="0.0.0.0"))