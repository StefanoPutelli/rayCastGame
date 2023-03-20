import { useEffect, useRef } from "react";
import { Map2D, Screen, RayCaster } from "./comp/classes";
import { map_text } from "./comp/conf/map.js";
import conf from "./comp/conf/config.json";

const Map = new Map2D(map_text);
const Ray = new RayCaster(conf, Map);
const screen = new Screen(conf);

//TODO: togliere lo shadowblur e cambaire invece il colore delle linee

function App() {

  const canvas = useRef({
    canvas: null,
    ctx: null
  });
  const keyPressed = useRef({})
  const mouseTurned = useRef(0);

  function checkTurn(){
    Ray.turn(mouseTurned.current);
    mouseTurned.current = 0;
  }

  useEffect(() => {
    const setCanvas = () => {
      canvas.current.canvas = document.getElementById('rayCanvas');
      canvas.current.ctx = canvas.current.canvas.getContext('2d');
    }
    window.addEventListener('load', setCanvas());
    return () => {
      window.removeEventListener('load', setCanvas());
    }
  }, []);


  useEffect(() => {
    if(!canvas.current.canvas) return;
    setInterval(() => {
      Ray.move(keyPressed.current,conf.player_speed);
      checkTurn();
      screen.drawScreen(canvas.current.ctx,canvas.current.canvas, Ray.rayCastInTheFov(), Map);
    }, 1000/conf.max_fps);
    return () => {
      clearInterval();
    }
  }, []);

  useEffect(() => {
    if(!canvas.current.canvas) return;
    const handleMouseMove = (e) => {
      mouseTurned.current += e.movementX * -0.02;
    }
    const handleKeyDown = (e) => {
      keyPressed.current[e.key] = true;
    }
    const handleKeyUp = (e) => {
      delete keyPressed.current[e.key];
    }
    async function lockPointer() {
      canvas.current.canvas.requestPointerLock({
          unadjustedMovement: true,
        });
    }
    const handleResize = () => {
      canvas.current.canvas.height = window.innerHeight;
      canvas.current.canvas.width = window.innerWidth;
      Ray.setDimensions(window.innerWidth,  window.innerHeight, conf.FOV);
      screen.setDimensions(window.innerWidth,  window.innerHeight, conf.FOV);
    }
    window.addEventListener('mousemove', handleMouseMove);
    window.addEventListener('keydown', handleKeyDown);
    canvas.current.canvas.addEventListener("click", lockPointer);
    window.addEventListener('resize', handleResize);
    window.addEventListener('load', handleResize);
    window.addEventListener('keyup', handleKeyUp);
    return () => {
      window.removeEventListener('keyup', handleKeyUp);
      window.removeEventListener('mousemove', handleMouseMove);
      window.removeEventListener('keydown', handleKeyDown);
      canvas.current.canvas.removeEventListener("click", lockPointer);
      window.removeEventListener('resize', handleResize);
      window.removeEventListener('load', handleResize);
    }
  }, []);




  return (
    <div className="App"><canvas id="rayCanvas" style={{ backgroundColor: "black", display: "block" }} /></div>
  );
}

export default App;
