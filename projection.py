#!/usr/bin/env python
# -*- coding: utf-8 -*-
import pygame
from pygame.locals import *
import sys
import math

import logging
from pygame import display
import signal
import time

def handler(signum, frame):
  """Why is systemd sending sighups? I DON'T KNOW."""
  logging.warning("Got a {} signal. Doing nothing".format(signum))

signal.signal(signal.SIGHUP, handler)
signal.signal(signal.SIGTERM, handler)
signal.signal(signal.SIGCONT, handler)

logging.warning("About to start display.")
try:
  display.init()   # hups
except Exception as ex:
  logging.warning("Got any exception: %s " % ex)

logging.warning("Quitting in 60")
time.sleep(60)


def main():
    #pygame.init()                                             # Pygameの初期化
    pygame.mixer.init()
    screen = pygame.display.set_mode((1200, 700), FULLSCREEN)  # フルスクリーン（解除時は大きさ400*300の画面）
    pygame.display.set_caption("Test")                        # タイトルバーに表示する文字

    fullscreen_flag =True

    count = 0
    a = 3.14159/360
    deg = 0
    rad = 0
    radius = 60

    while (1):
        screen.fill((0,0,0))        # 画面を黒色(#000000)に塗りつぶし
       
        count += a/3.14
        rad += count
        y = int(radius * math.sin(rad)); x = int(radius * math.cos(rad))
 
        pygame.draw.circle(screen, (255,0,0), (x+30, y+180), radius)
        pygame.draw.circle(screen, (0,255,0), (x+610,y+180), radius)
        pygame.draw.circle(screen, (0,0,255), (x+1150,y+180), radius)

        pygame.draw.circle(screen, (122,122,122), (x+30,y+580), radius)
        pygame.draw.circle(screen, (255,255,0), (x+610,y+580), radius)
        pygame.draw.circle(screen, (0,255,255), (x+1150,y+580), radius)

        pygame.display.update()     # 画面を更新

	# イベント処理
        for event in pygame.event.get():
            if event.type == QUIT:  # 閉じるボタンが押されたら終了
                pygame.quit()       # Pygameの終了(画面閉じられる)
                sys.exit()
            elif event.type == KEYDOWN and event.key == K_F2:
                # F2キーでフルスクリーンモードへの切り替え
                fullscreen_flag = not fullscreen_flag
                if fullscreen_flag:
                    screen = pygame.display.set_mode(SCR_RECT.size, FULLSCREEN, 32)
                else:
                    screen = pygame.display.set_mode(SCR_RECT.size, 0, 32)


#if __name__ == "__main__":
main()