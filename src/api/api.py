from fastapi import FastAPI, Request, WebSocket, WebSocketDisconnect
from websockets.exceptions import ConnectionClosed
from fastapi.templating import Jinja2Templates
from fastapi.middleware.cors import CORSMiddleware
from src.api.robot import (
    Position2D,
    RobotControl,
    DynamixelMotor,
    DYNAMIXEL_MX_12_ADDR_CONFIG,
)
from pygame import mixer
import asyncio
import cv2
import logging

logger = logging.getLogger(__file__)

app = FastAPI()
camera = cv2.VideoCapture(0)
templates = Jinja2Templates(directory="templates")
mixer.init()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],  # Allow all HTTP methods
    allow_headers=["*"],  # Allow all headers
)

motors = [
    DynamixelMotor(10, DYNAMIXEL_MX_12_ADDR_CONFIG, Position2D(0, 0, 0)),
    DynamixelMotor(11, DYNAMIXEL_MX_12_ADDR_CONFIG, Position2D(0, 0, 0)),
    DynamixelMotor(12, DYNAMIXEL_MX_12_ADDR_CONFIG, Position2D(0, 0, 0), True),
    DynamixelMotor(13, DYNAMIXEL_MX_12_ADDR_CONFIG, Position2D(0, 0, 0), True),
]
ctrl = RobotControl("/dev/ttyUSB0", 1, motors)

try:
    ctrl.init(1000000)
except:
    logger.error(
        "Unable to connect to Dynamixel, continuing without motor connection..."
    )


@app.get("/status")
def status():
    return {"status": "online"}


# https://stackoverflow.com/a/70626324
@app.websocket("/ws")
async def get_stream(websocket: WebSocket):
    await websocket.accept()
    try:
        while True:
            success, frame = camera.read()
            if not success:
                break
            else:
                ret, buffer = cv2.imencode(".jpg", frame)
                await websocket.send_bytes(buffer.tobytes())
            await asyncio.sleep(0.03)
    except (WebSocketDisconnect, ConnectionClosed):
        print("Client disconnected")


@app.post("/command")
async def command(request: Request):
    data = await request.json()

    x_val = -data.get("left", 0) + data.get("right", 0)
    y_val = -data.get("down", 0) + data.get("up", 0)
    rl = data.get("rotateleft", 0)
    rr = data.get("rotateright", 0)

    ctrl.set_velocity(Position2D(x_val, y_val, rr - rl))

    return {"status": "success"}


@app.post("/sfx")
async def command(request: Request):
    data = await request.json()

    if data.get("sfx") == "bloop":
        mixer.music.load("./assets/bloop.mp3")
    elif data.get("sfx") == "bong":
        mixer.music.load("./assets/bong.mp3")
    mixer.music.play()

    return {"status": "success"}
