import esphome.config_validation as cv
import pytest
from esphome.components import vfd_hcs_12ss59t
from esphome.yaml_util import dump_config

def test_minimal_config():
    config = {
        "platform": "vfd_hcs_12ss59t",
        "id": "vfd_display",
        "pin_reset": 5,
        "pin_vdon": 4,
        "pin_cs": 15,
    }
    validated = vfd_hcs_12ss59t.CONFIG_SCHEMA(config)
    assert validated["pin_reset"] == 5
    assert validated["pin_vdon"] == 4
    assert validated["pin_cs"] == 15
    assert validated["brightness"] == 1.0

def test_brightness_config():
    config = {
        "platform": "vfd_hcs_12ss59t",
        "id": "vfd_display",
        "pin_reset": 5,
        "pin_vdon": 4,
        "pin_cs": 15,
        "brightness": "50%",
    }
    validated = vfd_hcs_12ss59t.CONFIG_SCHEMA(config)
    assert abs(validated["brightness"] - 0.5) < 0.01

def test_yaml_dump():
    config = {
        "platform": "vfd_hcs_12ss59t",
        "id": "vfd_display",
        "pin_reset": 5,
        "pin_vdon": 4,
        "pin_cs": 15,
        "brightness": "80%",
    }
    validated = vfd_hcs_12ss59t.CONFIG_SCHEMA(config)
    yaml = dump_config(validated)
    assert "platform: vfd_hcs_12ss59t" in yaml
    assert "brightness: 0.8" in yaml
