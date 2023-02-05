import re
from pathlib import Path
from enum import Flag

moduleRoot = Path(__file__).parent

with open(moduleRoot / "KbEKeyToKeySymMap", "r") as fh:
    KbEKeyToKeySymMapData = fh.read()

KbEKeyToKeySymMap = dict(
    re.findall(
        "^(.+?)\s(.+)$", 
        KbEKeyToKeySymMapData, 
        re.M)
    )

with open(moduleRoot / "xmodmapDump", "r") as fh:
    KeySymToKeyCodeMapData = fh.read()

# modifiers for each keySym Column, note Mode_switch is AltGr
# Key
# Shift + Key
# Mode_switch + Key
# Shift + Mode_switch + Key
# ISO_Level3_Shift + Key
# Shift + ISO_Level3_Shift + Key


class Modifiers(Flag):
    NO_MOD = 0
    SHIFT = 1
    MODE_SWICH = 2
    ISO_Level3_SHIFT = 4

KeySymToKeyCodeMapColumns = [
    Modifiers.NO_MOD,
    Modifiers.SHIFT,
    Modifiers.MODE_SWICH,
    Modifiers.SHIFT | Modifiers.MODE_SWICH,
    Modifiers.ISO_Level3_SHIFT,
    Modifiers.SHIFT | Modifiers.ISO_Level3_SHIFT,
]


KeySymToKeyCodeMap = {}
for keyCode, keySymsText in re.findall("^keycode\s+(\d+)\s=\s(.*)$", KeySymToKeyCodeMapData, re.M):

    for n, keySym in enumerate(keySymsText.split(" ")):

        if keySym not in KeySymToKeyCodeMap and n < len(KeySymToKeyCodeMapColumns):
            KeySymToKeyCodeMap[keySym] = (
                keyCode,
                KeySymToKeyCodeMapColumns[n]
                )

def KbEKeytoKeyCode(KbEKey):

    try:
        keySym = KbEKeyToKeySymMap[KbEKey]

        keyCode = KeySymToKeyCodeMap[keySym]

        return keyCode

    except KeyError:
        return (None, None)