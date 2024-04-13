from fastapi import FastAPI, Request, WebSocket, WebSocketDisconnect
from websockets.exceptions import ConnectionClosed
from fastapi.templating import Jinja2Templates
from fastapi.middleware.cors import CORSMiddleware
from robot import (
    Position2D,
    RobotControl,
    DynamixelMotor,
    DYNAMIXEL_MX_12_ADDR_CONFIG,
)
from pygame import mixer
import asyncio
import cv2

app = FastAPI()
camera = cv2.VideoCapture(1, cv2.CAP_DSHOW)
templates = Jinja2Templates(directory="templates")
mixer.init()

origins = [
    "http://localhost:3000",
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],  # Allow all HTTP methods
    allow_headers=["*"],  # Allow all headers
)

motors = [
    DynamixelMotor(10, DYNAMIXEL_MX_12_ADDR_CONFIG, Position2D(0, 0, 0)),
    DynamixelMotor(11, DYNAMIXEL_MX_12_ADDR_CONFIG, Position2D(0, 0, 0), True),
]
ctrl = RobotControl("/dev/ttyUSB0", 1, motors)
ctrl.init(1000000)


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

    ctrl.set_velocity(Position2D(x_val, y_val, 0))

    return {"status": "success"}

@app.post("/sfx")
async def command(request: Request):
    data = await request.json()

    if data.get("sfx") == "bloop":
        mixer.music.load("./bloop.mp3")
    elif data.get("sfx") == "bong":
        mixer.music.load("./bong.mp3")
    mixer.music.play()

    return {"status": "success"}