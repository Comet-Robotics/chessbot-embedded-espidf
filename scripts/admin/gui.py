import dearpygui.dearpygui as dpg

from admin.robot_connection import DEFAULT_IP, RobotConnection
from admin.gui_decl import GuiRobot

def draw_robot_window():
    global robot_ip_box, robot_mac_text

    with dpg.window(label="Robot Connection"):
        dpg.add_text("Robot Connection Information")
        GuiRobot.ip_box = dpg.add_input_text(label="IP", default_value=DEFAULT_IP)

        def connect_to_robot(sender, app_data, user_data):
            RobotConnection.active = RobotConnection(dpg.get_value(user_data))

        dpg.add_button(label="Connect", callback=connect_to_robot, user_data=GuiRobot.ip_box)
        GuiRobot.mac_text = dpg.add_text("Robot MAC Address: Empty")
        GuiRobot.conn_status_text = dpg.add_text("Connection Status: Empty")