import { useEffect, useRef } from "react";
import { Map2D, Screen, RayCaster } from "./comp/classes";
import { map_text } from "./comp/conf/map.js";
import conf from "./comp/conf/config.json";

const Map = new Map2D(map_text);
const Ray = new RayCaster(conf, Map);
const screen = new Screen(conf);

function App() {

  const height = useRef(window.innerHeight);
  const width = useRef(window.innerWidth);
  const canvas = useRef(null);

  useEffect(() => {
    const setCanvas = () => {
      canvas.current = document.getElementById('rayCanvas');
    }
    window.addEventListener('load', setCanvas());
    return () => {
      window.removeEventListener('load', setCanvas());
    }
  }, []);


  useEffect(() => {
    if(!canvas.current) return;
    setInterval(() => {
      canvas.current.width = width.current;
      canvas.current.height = height.current;
      screen.drawScreen(canvas.current, Ray.rayCastInTheFov(), Map);
    }, 1000/60);
    return () => {
      clearInterval();
    }
  }, []);

  useEffect(() => {
    if(!canvas.current) return;
    const handleMouseMove = (e) => {
      Ray.turn(e.movementX * 0.02);
    }
    const handleKeyDown = (e) => {
      Ray.move(e.key)
    }
    async function lockPointer() {
      canvas.current.requestPointerLock({
          unadjustedMovement: true,
        });
    }
    const handleResize = () => {
      height.current = window.innerHeight;
      width.current = window.innerWidth;
      Ray.setDimensions(width.current, height.current, conf.FOV);
      screen.setDimensions(width.current, height.current, conf.FOV);
    }
    const setInitialDimensions = () => {
      Ray.setDimensions(width.current, height.current, conf.FOV)
      screen.setDimensions(width.current, height.current, conf.FOV)
    }
    window.addEventListener('mousemove', handleMouseMove);
    window.addEventListener('keydown', handleKeyDown);
    canvas.current.addEventListener("click", lockPointer);
    window.addEventListener('resize', handleResize);
    window.addEventListener('load', setInitialDimensions);
    return () => {
      window.removeEventListener('mousemove', handleMouseMove);
      window.removeEventListener('keydown', handleKeyDown);
      canvas.current.removeEventListener("click", lockPointer);
      window.removeEventListener('resize', handleResize);
      window.removeEventListener('load', setInitialDimensions);
    }
  }, []);




  return (
    <div className="App"><canvas id="rayCanvas" style={{ backgroundColor: "black", display: "block" }} /></div>
  );
}

export default App;
