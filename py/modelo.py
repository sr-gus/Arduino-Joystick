class RobotModelo:
    def __init__(self, x, y, frame, velocidad):
        self.x = x
        self.y = y
        self.velocidad = velocidad
        self.pps = 100
        self.frame = frame
        self.reloj = 0

    def get_posicion(self):
        return (self.x, self.y)

    def set_posicion(self, nueva_posicion):
        self.x = nueva_posicion[0]
        self.y = nueva_posicion[1]

    def get_frame(self):
        return self.frame

    def get_reloj(self):
        return self.reloj

    def get_velocidad(self):
        return self.velocidad

    def set_velocidad(self, nueva_velocidad):
        self.velocidad = nueva_velocidad

    def siguiente_frame(self):
        self.reloj = 0
        self.frame += 1
        self.frame %= 4

    def detectar_colision(self, objetos):
        for objeto in objetos:
            if abs(self.get_posicion()[0] - objeto.get_posicion()[0]) < 16 \
              and abs(self.get_posicion()[1] - objeto.get_posicion()[1]) < 16:
              return True
        return False
