"use client"
import { useRef, useEffect, useInsertionEffect } from "react";

export default function Home() {
  const wsRef = useRef(null);
  const canvasRef = useRef(null);

  useInsertionEffect(() => {
    const ws = new WebSocket('ws://localhost:8000/ws');
    wsRef.current = ws;

    ws.onmessage = (event) => {
      if (event.data instanceof Blob) {
        const reader = new FileReader();
        reader.onload = () => {
          const img = new Image();
          img.onload = () => {
            const canvas = canvasRef.current;
            const context = canvas?.getContext('2d');
            if (context) {
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

    // Clean up WebSocket connection on component unmount
    return () => {
      ws.close();
    };
  }, []);

  useEffect(() => {
    const canvas = canvasRef.current;
    const context = canvas?.getContext('2d');
  }, []);
  
  return (
    <main className="min-h-screen w-full">
      <div className="flex flex-col items-center justify-center min-h-screen max-h-screen overflow-hidden w-full">
        <canvas ref={canvasRef} className="h-3/5 w-3/5" alt="Trenton" />
      </div>
    </main>
  );
}
