import pygame, os, sys
from pygame.locals import *
from modelo import RobotModelo

class RobotVista:
    def __init__(self, ruta_imagen):
        self.imagen = pygame.image.load(ruta_imagen)

    def dibujar(self, pantalla, modelo):
        esqueleto = Rect(modelo.get_frame()*32, 0, 32, 32)
        pantalla.blit(self.imagen, modelo.get_posicion(), esqueleto)

class RadarVista:
    def __init__(self, imagen_radar, imagen_blip):
        self.imagen_radar = pygame.image.load(imagen_radar)
        self.imagen_blip = pygame.image.load(imagen_blip)

    def dibujar(self, pantalla, modelos):
        for modelo in modelos:
            x = (modelo.x/10.0)+1
            y = (modelo.y/10.0)+1
            pantalla.blit(self.imagen_blip, (x, y))
        pantalla.blit(self.imagen_radar, (0, 0))
