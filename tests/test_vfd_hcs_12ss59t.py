import pytest
from esphome import config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_CS_PIN,
    CONF_RESET_PIN,
    CONF_VDON_PIN,
    CONF_BRIGHTNESS,
    CONF_UPDATE_INTERVAL,
)
from components.vfd_hcs_12ss59t import vfd_hcs_12ss59t

def test_schema_valid_minimal():
    config = {
        CONF_ID: "vfd1",
        CONF_CS_PIN: 5,
        CONF_BRIGHTNESS: 4,
        CONF_UPDATE_INTERVAL: "500ms",
    }
    # Should not raise
    vfd_hcs_12ss59t.validate_config(config)

def test_schema_invalid_brightness():
    config = {
        CONF_ID: "vfd1",
        CONF_CS_PIN: 5,
        CONF_BRIGHTNESS: 20,  # Invalid, max 15
        CONF_UPDATE_INTERVAL: "500ms",
    }
    with pytest.raises(cv.Invalid):
        vfd_hcs_12ss59t.validate_config(config)

def test_pin_configuration():
    config = {
        CONF_ID: "vfd1",
        CONF_CS_PIN: 12,
        CONF_RESET_PIN: 14,
        CONF_VDON_PIN: 16,
        CONF_BRIGHTNESS: 8,
        CONF_UPDATE_INTERVAL: "1s",
    }
    # Should not raise
    vfd_hcs_12ss59t.validate_config(config)

def test_brightness_clamping():
    # Simulate the clamp_brightness logic
    clamp = vfd_hcs_12ss59t.clamp_brightness
    assert clamp(-5) == 0
    assert clamp(0) == 0
    assert clamp(7) == 7
    assert clamp(15) == 15
    assert clamp(20) == 15