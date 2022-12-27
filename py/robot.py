import pygame, os, sys
from pygame.locals import *
from modelo import RobotModelo
from vista import RobotVista, RadarVista
from controlador import RobotControlador, RobotsEnemigosControlador
from generador import RobotsEnemigosGenerador
from threading import Thread
from Joystick import *
import os.path

PATH = os.path.dirname(__file__)

pygame.init()
reloj = pygame.time.Clock()
pantalla = pygame.display.set_mode((640,480))

fondo = pygame.Color(13, 59, 102)

robot_modelo = RobotModelo((640-32)/2, (480-32)/2, 200, (0, 0))
robot_vista = RobotVista(PATH + '/../img/robot.png')
robot_controlador = RobotControlador()
radar_vista = RadarVista(PATH +'/../img/radar.png', PATH + '/../img/blip.png')
robot_enemigo_vista = RobotVista(PATH + '/../img/evil_robot.png')
robots_enemigos_generador = RobotsEnemigosGenerador()
robots_enemigos_controlador = RobotsEnemigosControlador(robots_enemigos_generador.get_robots())

t_trans = 0

read_joystick = Thread(target=collect_arduino_data)
read_joystick.start()

while True:
    for evento in pygame.event.get():
        if evento.type == QUIT:
            pygame.quit()
            sys.exit()

    if not buttons['P']:
        delta_t = t_trans/1000
        robots_enemigos_generador.actualizar(delta_t)
        robot_controlador.actualizar(delta_t, robot_modelo)
        robots_enemigos_controlador.actualizar(delta_t)

        pantalla.fill(fondo)
        robot_vista.dibujar(pantalla, robot_modelo)
        for modelo in robots_enemigos_generador.get_robots():
            robot_enemigo_vista.dibujar(pantalla, modelo)
        modelos = [robot_modelo]
        modelos.extend(robots_enemigos_generador.get_robots())
        radar_vista.dibujar(pantalla, modelos)

        colision = robot_modelo.detectar_colision(robots_enemigos_generador.get_robots())
        if colision:
            pantalla.fill(fondo)

            fuente = pygame.font.SysFont('comicsans', 90)

            imagen = pygame.image.load(PATH + '/../img/game-over.png')
            ancho_imagen = imagen.get_width()
            alto_imagen = imagen.get_height()
            imagen_esqueleto = ((640 - ancho_imagen) / 2,
                                (480 - alto_imagen) / 2)

            pantalla.blit(imagen, imagen_esqueleto)
            pygame.display.update()
            pygame.time.delay(3000)
            break
        pygame.display.update()
        t_trans = reloj.tick(60)

    else:
        pantalla.fill(fondo)

        fuente = pygame.font.SysFont('comicsans', 90)

        mensaje = fuente.render('Pausa', 1, (0, 0, 0))
        ancho_mensaje = mensaje.get_width()
        alto_mensaje = mensaje.get_height()
        mensaje_esqueleto = ((640 - ancho_mensaje) / 2,
                            (480 - alto_mensaje) / 2)

        pantalla.blit(mensaje, mensaje_esqueleto)
        pygame.display.update()
