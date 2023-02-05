import eventlet
from flask import Flask, send_file
from flask_socketio import SocketIO
from input_relay import RPC_Client, Action
from pathlib import Path
from key_mapping import KbEKeytoKeyCode, Modifiers

projectRoot = Path(__file__).parent
staticDir = projectRoot / 'app_static'

app = Flask(__name__,
            static_url_path='', 
            static_folder= staticDir)

socketio = SocketIO(app)

@app.route('/')
def index():
    return send_file(staticDir / 'index.html')



    # Key
    # Shift + Key
    # Mode_switch + Key
    # Shift + Mode_switch + Key
    # ISO_Level3_Shift + Key
    # Shift + ISO_Level3_Shift + Key


@socketio.on('relMotion')
def handle_raw_input(event):

    rpc_cli = RPC_Client()

    rpc_cli.relMotion(int(event['x']), int(event['y']))

@socketio.on('buttonPress')
def handle_raw_input(event):

    rpc_cli = RPC_Client()

    rpc_cli.buttonPress(event['button'], Action.PRESS.value)

@socketio.on('keyPress')
def handle_raw_input(event):

    rpc_cli = RPC_Client()

    keyCode, modifiers = KbEKeytoKeyCode(event['key'])
    print(keyCode, modifiers)

    if modifiers is not None:
        for modifier in Modifiers:

            if modifier == Modifiers.SHIFT:
                shiftKeyCode = KbEKeytoKeyCode("Shift")
                # rpc_cli.keyDown(shiftKeyCode)

            # same as modeSwitch see https://unix.stackexchange.com/questions/55076/what-is-the-mode-switch-modifier-for
            if modifier == Modifiers.MODE_SWICH or modifier == Modifiers.ISO_Level3_SHIFT:
                modeSwitchKeyCode = KbEKeytoKeyCode("AltGraph")
                # rpc_cli.keyDown(modeSwitchKeyCode)

    # need to force keyCode to int
    rpc_cli.keyPress(int(keyCode), Action.PRESS.value)

    if modifiers is not None:
        for modifier in Modifiers:

            if modifier == Modifiers.SHIFT:
                shiftKeyCode = KbEKeytoKeyCode("Shift")
                # rpc_cli.keyUp(shiftKeyCode)

            if modifier == Modifiers.MODE_SWICH or modifier == Modifiers.ISO_Level3_SHIFT:
                modeSwitchKeyCode = KbEKeytoKeyCode("AltGraph")
                # rpc_cli.keyUp(modeSwitchKeyCode)




eventlet.wsgi.server(
    eventlet.listen(('0.0.0.0', 4242)), app)
