import json
from sys import argv


class FlipperAppType_:
    SERVICE = "SERVICE"
    SYSTEM = "SYSTEM"
    APP = "APP"
    PLUGIN = "PLUGIN"
    DEBUG = "DEBUG"
    ARCHIVE = "ARCHIVE"
    SETTINGS = "SETTINGS"
    STARTUP = "STARTUP"
    EXTERNAL = "EXTERNAL"
    METAPACKAGE = "METAPACKAGE"


FlipperAppType = FlipperAppType_()


def App(appid="flippulator_app", apptype=FlipperAppType.EXTERNAL,
        name="Flippulator App", entry_point="flippulator_main",
        flags=[], cdefines={}, requires=[], conflicts=[],
        stack_size=1024, icon="", order=999, sdk_headers=[],
        targets=["all"], sources=["*.c*"], fap_version="0.1",
        fap_icon="", fap_libs=[], fap_category="Tools",
        fap_description="An app compiled with flippulator.",
        fap_author="Flippulator", fap_weburl="",
        fap_icon_assets="", fap_extbuild=[]):
    print(json.dumps({
        "appid": appid,
        "apptype": apptype,
        "name": name,
        "entry_point": entry_point,
        "flags": flags,
        "cdefines": cdefines,
        "requires": requires,
        "conflicts": conflicts,
        "stack_size": stack_size,
        "icon": icon,
        "order": order,
        "sdk_headers": sdk_headers,
        "targets": targets,
        "sources": sources,
        "fap_version": fap_version,
        "fap_icon": fap_icon,
        "fap_libs": fap_libs,
        "fap_category": fap_category,
        "fap_description": fap_description,
        "fap_author": fap_author,
        "fap_weburl": fap_weburl,
        "fap_icon_assets": fap_icon_assets,
        "fap_extbuild": fap_extbuild
    }))


exec(open(argv[-1]).read())
