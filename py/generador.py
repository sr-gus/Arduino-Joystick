from modelo import RobotModelo
import random

class RobotsEnemigosGenerador:
    def __init__(self, tiempo_generacion=1, max_robots=10):
        self.robots = []
        self.tiempo_generacion = tiempo_generacion
        self.max_robots = max_robots
        self.contador = 0

    def get_robots(self):
        return self.robots

    def actualizar(self, delta_tiempo):
        self.contador += delta_tiempo
        if self.contador >= self.tiempo_generacion and len(self.robots) < self.max_robots:
            self.contador = 0
            x = random.randint(36,600)
            y = random.randint(36,440)
            frame = random.randint(0,3)
            vx =- 50 + random.random()*200
            vy =- 50 + random.random()*200
            nuevo_robot = RobotModelo(x, y, frame, (vx, vy))
            self.robots.append(nuevo_robot)
