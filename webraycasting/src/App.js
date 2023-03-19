
import { useEffect, useRef, useState } from "react";
import { Map2D, Screen, RayCaster } from "./comp/classes";
import { map_text } from "./comp/conf/map.js";
import conf from "./comp/conf/config.json";

const Map = new Map2D(map_text);
const Ray = new RayCaster(conf, Map);
const screen = new Screen(conf);

function App() {

  const height = useRef(window.innerHeight);
  const width = useRef(window.innerWidth);

  useEffect(() => {
    const canvas = document.getElementById('rayCanvas');
    setInterval(() => {
      Ray.rayCastInTheFov();
      screen.drawScreen(canvas, Ray.getFovArray(), Map);
      Ray.turn(-0.6);
    }, 1000);
    return () => {
      clearInterval();
    }
  }, []);

  useEffect(() => {
    const handleResize = () => {
      height.current = window.innerHeight;
      width.current = window.innerWidth;
      Ray.setDimensions(width.current, height.current, conf.FOV);
      screen.setDimensions(width.current, height.current);
    }
    const setInitialDimensions = () => {
      Ray.setDimensions(width.current, height.current, conf.FOV)
      screen.setDimensions(width.current, height.current)
    }
    window.addEventListener('resize', handleResize);
    window.addEventListener('load', setInitialDimensions);
    return () => {
      window.removeEventListener('resize', handleResize);
      window.removeEventListener('load', setInitialDimensions);
    }
  }, []);



  return (
    <div className="App">
      <canvas id="rayCanvas" width={width.current} height={height.current} style={{ backgroundColor: "black" }} />
    </div>
  );
}

export default App;
