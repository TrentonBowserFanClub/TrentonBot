"use client"
import { useRef, useEffect, useInsertionEffect, useState } from "react";
import { FaArrowDown, FaArrowLeft, FaArrowRight, FaArrowUp  } from "react-icons/fa";
import { FaArrowRotateLeft, FaArrowRotateRight } from "react-icons/fa6";
// @ts-ignore
import useSound from 'use-sound';
// @ts-ignore
import bong from '../../public/bong.mp3';
// @ts-ignore
import bloop from '../../public/bloop.mp3';

export default function Home() {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const [bongSfx] = useSound(bong);
  const [bloopSfx] = useSound(bloop);

  const playBong = () => {
    fetch(`http://${ip}:8000/sfx`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        sfx: 'bong',
      }),
    });
    bongSfx();
  };

  const playBloop = () => {
    fetch(`http://${ip}:8000/sfx`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        sfx: 'bloop',
      }),
    });
    bloopSfx();
  };

  const [left, setLeft] = useState(0);
  const [up, setUp] = useState(0);
  const [right, setRight] = useState(0);
  const [down, setDown] = useState(0);
  const [rotateleft, setRotateLeft] = useState(0);
  const [rotateright, setRotateRight] = useState(0);
  const [ip, setIp] = useState("localhost");

  useInsertionEffect(() => {
    const ws = new WebSocket(`ws://${ip}:8000/ws`);
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
        case "w":
          setUp(1);
          break;
        case "s":
          setDown(1);
          break;
        case "a":
          setLeft(1);
          break;
        case "d":
          setRight(1);
          break;
        case "q":
          setRotateLeft(1);
          break;
        case "e":
          setRotateRight(1);
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
        case "w":
          setUp(0);
          break;
        case "s":
          setDown(0);
          break;
        case "a":
          setLeft(0);
          break;
        case "d":
          setRight(0);
          break;
        case "q":
          setRotateLeft(0);
          break;
        case "e":
          setRotateRight(0);
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
    fetch(`http://${ip}:8000/command`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        left,
        up,
        right,
        down,
        rotateleft,
        rotateright,
      }),
    });
  }, [down, left, right, up, rotateleft, rotateright])

  return (
    <main className="min-h-screen w-full">
      <div className="flex flex-col items-center justify-center min-h-screen max-h-screen overflow-hidden w-full">
        <div className="grid grid-cols-6 w-full">
          <div className="col-span-4">
            <canvas ref={canvasRef} className="w-full" />
          </div>
          <div className="col-span-2 bg-gradient-to-br from-red to-yellow">
            <a href="https://www.linkedin.com/in/trentonbowser/"
            rel="noreferrer"
            target="_blank"
            >
              <div className="bottom-3 right-3 absolute fill-blue hover:cursor-pointer hover:fill-white">
                <svg version="1.0" xmlns="http://www.w3.org/2000/svg"
                width="50.000000pt" height="50.000000pt" viewBox="0 0 440.000000 448.000000"
                preserveAspectRatio="xMidYMid meet">
                <g transform="translate(0.000000,448.000000) scale(0.100000,-0.100000)"
                stroke="none">
                <path d="M870 3755 l0 -115 228 -2 227 -3 3 -652 2 -653 140 0 140 0 2 653 3
                652 233 3 232 2 0 115 0 115 -605 0 -605 0 0 -115z"/>
                <path d="M2360 3089 l0 -760 418 3 c406 3 419 4 482 26 195 69 284 201 285
                422 0 97 -3 110 -31 170 -46 97 -126 167 -227 200 -21 6 -37 16 -35 21 2 5 26
                26 53 47 124 93 164 244 109 404 -21 63 -89 137 -157 171 -92 47 -191 57 -564
                57 l-333 0 0 -761z m700 509 c66 -34 84 -66 88 -153 5 -95 -19 -144 -87 -177
                -41 -21 -64 -23 -223 -26 l-178 -4 0 191 0 191 178 0 c165 0 181 -2 222 -22z
                m37 -590 c30 -7 60 -25 88 -52 35 -34 45 -52 55 -100 23 -114 -16 -212 -103
                -259 -39 -20 -57 -22 -259 -25 l-218 -4 0 226 0 226 196 0 c114 0 214 -5 241
                -12z"/>
                <path d="M2821 2125 c-196 -44 -320 -127 -415 -275 -78 -121 -114 -242 -123
                -420 -21 -405 138 -686 446 -793 75 -26 104 -30 226 -34 239 -8 377 38 499
                168 67 70 127 178 130 231 1 27 -6 32 -103 83 -58 30 -110 55 -117 55 -6 0
                -21 -28 -34 -62 -28 -78 -100 -157 -173 -189 -45 -20 -68 -24 -167 -24 -106 0
                -120 2 -177 29 -150 69 -223 226 -223 477 0 190 45 330 131 411 74 70 139 92
                269 92 86 0 116 -4 162 -23 63 -24 124 -80 162 -146 12 -23 29 -39 38 -39 9 1
                64 30 122 64 l106 62 -36 69 c-41 78 -128 166 -202 203 -97 49 -179 67 -322
                72 -96 3 -154 0 -199 -11z"/>
                <path d="M1060 1370 l0 -760 150 0 150 0 0 305 0 305 288 0 c158 0 293 4 300
                8 9 6 12 38 10 123 l-3 114 -297 3 -298 2 0 210 0 210 325 0 325 0 0 120 0
                120 -475 0 -475 0 0 -760z"/>
                </g>
                </svg>
              </div>
            </a>
            <div className="flex flex-col items-center justify-center h-full">
              <div className="fixed top-4 right-4">
                <select className="h-6 bg-transparent text-blue font-bold text-lg text-right" onChange={e => {
                  setIp(e.target.value);
                  }}>
                  <option value="localhost">localhost</option>
                  <option value="192.168.42.121">192.168.42.121</option>
                </select>
              </div>
              <div className="mb-10 space-x-10">
                <button className=" hover:text-white text-3xl text-blue font-bold active:border-none" onClick={playBloop}>
                  BING
                </button>
                <button className=" hover:text-white text-3xl text-blue font-bold active:border-none" onClick={playBong}>
                  BONG
                </button>
              </div>
              <div className="grid grid-cols-3 w-1/3">
                <div className={rotateleft ? "border-green text-green" : "border-blue text-blue"}>
                  <div className="rounded-[50%] aspect-square flex justify-center items-center">
                    <FaArrowRotateLeft size={32}/>
                  </div>
                </div>
                <div className={up ? "border-green text-green" : "border-blue text-blue"}>
                  <div className="border-4 rounded-[50%] aspect-square flex justify-center items-center">
                    <FaArrowUp size={32}/>
                  </div>
                </div>
                <div className={rotateright ? "border-green text-green" : "border-blue text-blue"}>
                  <div className="rounded-[50%] aspect-square flex justify-center items-center">
                    <FaArrowRotateRight size={32}/>
                  </div>
                </div>
                <div className={left ? "border-green text-green" : "border-blue text-blue"}>
                  <div className="border-4 rounded-[50%] aspect-square flex justify-center items-center">
                    <FaArrowLeft size={32}/>
                  </div>
                </div>
                <div className="aspect-square"/>
                <div className={right ? "border-green text-green" : "border-blue text-blue"}>
                  <div className="border-4 rounded-[50%] aspect-square flex justify-center items-center">
                    <FaArrowRight size={32}/>
                  </div>
                </div>
                <div className="aspect-square"/>
                <div className={down ? "border-green text-green" : "border-blue text-blue"}>
                  <div className="border-4 rounded-[50%] aspect-square flex justify-center items-center">
                    <FaArrowDown size={32}/>
                  </div>
                </div>
                <div className="aspect-square"/>
              </div>
            </div>
          </div>
        </div>
      </div>
    </main>
  );
}
