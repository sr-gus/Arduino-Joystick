import pygame, os, sys
from pygame.locals import *
from modelo import RobotModelo
from Joystick import buttons

class RobotControlador:
    def actualizar(self, delta_tiempo, modelo):
        modelo.reloj += delta_tiempo
        if modelo.reloj > 0.125:
            modelo.siguiente_frame()

        distancia = self.multiplicar(modelo.get_velocidad(), delta_tiempo)
        posicion = modelo.get_posicion()
        x, y = self.sumar(posicion, distancia)
        vx, vy = modelo.get_velocidad()

        keys = pygame.key.get_pressed()

        if buttons['L'] or keys[K_LEFT]:
            if x > 0:
                vx = -modelo.pps
            else:
                x = 0
                vx = 0
        elif buttons['R'] or keys[K_RIGHT]:
            if x < 608:
                vx = modelo.pps
            else:
                x = 608
                vx = 0
        else:
            vx = 0

        if buttons['U'] or keys[K_UP]:
            if modelo.y > 0:
                vy = -modelo.pps
            else:
                y = 0
                vy = 0
        elif buttons['D'] or keys[K_DOWN]:
            if modelo.y < 448:
                vy = modelo.pps
            else:
                y = 448
                vy = 0
        else:
            vy = 0

        modelo.set_posicion((x, y))
        modelo.set_velocidad((vx, vy))

    def multiplicar(self, velocidad, delta_tiempo):
        x = velocidad[0]*delta_tiempo
        y = velocidad[1]*delta_tiempo
        return (x, y)

    def sumar(self, posicion, distancias):
        x = distancias[0]+posicion[0]
        y = distancias[1]+posicion[1]
        return (x, y)

class RobotsEnemigosControlador(RobotControlador):
    def __init__(self, modelos):
        self.robots = modelos

    def actualizar(self, delta_tiempo):
        for modelo in self.robots:
            modelo.reloj += delta_tiempo
            if modelo.get_reloj() >= 0.125:
                modelo.siguiente_frame()
            distancia = self.multiplicar(modelo.get_velocidad(), delta_tiempo)
            posicion = modelo.get_posicion()
            x, y = self.sumar(posicion, distancia)
            vx, vy = modelo.get_velocidad()
            if x <= 0:
                x = 0
                vx *= -1
            if y <= 0:
                y = 0
                vy *= -1
            if x > 640-32:
                x = 640-32
                vx *= -1
            if y > 480-32:
                y = 480-32
                vy *= -1
            modelo.set_posicion((x, y))
            modelo.set_velocidad((vx, vy))
