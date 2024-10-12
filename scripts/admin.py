import pip

def import_or_install(package):
    try:
        return __import__(package)
    except ImportError:
        pip.main(['install', package])
        return __import__(package)

dearpygui = import_or_install("dearpygui")
import dearpygui.dearpygui as dpg

import admin.gui as gui
from admin.robot_connection import RobotConnection

dpg.create_context()

gui.draw_robot_window()

dpg.create_viewport(title='ChessBot Local Admin', width=1000, height=700)
dpg.setup_dearpygui()
dpg.show_viewport()

while dpg.is_dearpygui_running():
    if RobotConnection.active is not None:
        RobotConnection.active.tick()

    dpg.render_dearpygui_frame()

dpg.destroy_context()