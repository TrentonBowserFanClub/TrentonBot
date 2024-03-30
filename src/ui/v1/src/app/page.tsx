"use client"
import { useRef, useEffect, useInsertionEffect, useState } from "react";

export default function Home() {
  const ws = new WebSocket('ws://localhost:8000/ws');
  const canvasRef = useRef<HTMLCanvasElement>(null);

  const [left, setLeft] = useState(0);
  const [up, setUp] = useState(0);
  const [right, setRight] = useState(0);
  const [down, setDown] = useState(0);

  useInsertionEffect(() => {
    ws.onmessage = (event) => {
      if (event.data instanceof Blob) {
        const reader = new FileReader();
        reader.onload = () => {
          const img = new Image();
          img.onload = () => {
            const canvas = canvasRef.current as HTMLCanvasElement;
            const context = canvas?.getContext('2d');
            if (context && canvas) {
              canvas.width = img.width;
              canvas.height = img.height;
              context.drawImage(img, 0, 0);
            }
          };
          img.src = reader.result as string;
        };
        reader.readAsDataURL(event.data);
      }
    };

    return () => {
      ws.close();
    };
  }, []);

  useEffect(() => {
    const canvas = canvasRef.current;
    const context = canvas?.getContext('2d');
  }, []);

  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
      switch (event.key) {
        case "ArrowUp":
          setUp(1);
          break;
        case "ArrowDown":
          setDown(1);
          break;
        case "ArrowLeft":
          setLeft(1);
          break;
        case "ArrowRight":
          setRight(1);
          break;
        default:
          break;
      }
    };

    document.addEventListener("keydown", handleKeyDown);

    return () => {
      document.removeEventListener("keydown", handleKeyDown);
    };
  }, []);

  useEffect(() => {
    const handleKeyUp = (event: KeyboardEvent) => {
      switch (event.key) {
        case "ArrowUp":
          setUp(0);
          break;
        case "ArrowDown":
          setDown(0);
          break;
        case "ArrowLeft":
          setLeft(0);
          break;
        case "ArrowRight":
          setRight(0);
          break;
        default:
          break;
      }
    }

    document.addEventListener("keyup", handleKeyUp);

    return () => {
      document.removeEventListener("keyup", handleKeyUp);
    }
  }, [])

  useEffect(() => {
    if (left === 1 || up === 1 || right === 1 || down === 1) {
      fetch('http://localhost:8000/command', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          left,
          up,
          right,
          down
        }),
      });
    }
  }, [down, left, right, up])

  return (
    <main className="min-h-screen w-full">
      <div className="flex flex-col items-center justify-center min-h-screen max-h-screen overflow-hidden w-full">
        <div className="grid grid-cols-6 w-full">
          <div className="col-span-4">
            <canvas ref={canvasRef} className="w-full" />
          </div>
          <div className="col-span-2 bg-gradient-to-br from-red to-yellow">
            <div className="flex flex-col items-center justify-center h-full">
              <button
                className="arrow-button"
                onClick={() => setDirection("up")}
              >
                Up
              </button>
              <button
                className="arrow-button"
                onClick={() => setDirection("down")}
              >
                Down
              </button>
              <button
                className="arrow-button"
                onClick={() => setDirection("left")}
              >
                Left
              </button>
              <button
                className="arrow-button"
                onClick={() => setDirection("right")}
              >
                Right
              </button>
            </div>
          </div>
        </div>
      </div>
    </main>
  );
}
