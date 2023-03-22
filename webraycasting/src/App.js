import { useEffect, useRef, useState } from "react";
import { Map2D, Screen, Player} from "./comp/classes";
import map_text from "./comp/conf/map.txt";
import conf from "./comp/conf/config.json";

const reader = new FileReader();
const Map = new Map2D(map_text);
const Ray = new Player(conf, Map, {x:28 , y:20}, 90);
const screen = new Screen(conf);

//TODO: togliere lo shadowblur e cambaire invece il colore delle linee

function App() {

  const [loaded, setLoaded] = useState(false);

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
      screen.drawMap(canvas.current.ctx, Map.map2D, Ray.getPlayerPosition().x, Ray.getPlayerPosition().y);
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
    <div className="App">
      <canvas id="rayCanvas" style={{ backgroundColor: "black", display: "block", position: "absolute", top: "0px", left: "0px" }} />
      <canvas id="miniMap" style={{ display: "block", position: "absolute", top: "0px", left: "0px" }} />
      <div style={{position: "absolute", right: "0", top: "0", color:"white"}}>
        <p>WASD to move</p>
        <p>Mouse to turn</p>
        <p>Click to lock mouse</p>
      </div>
    </div>
  );
}

export default App;
