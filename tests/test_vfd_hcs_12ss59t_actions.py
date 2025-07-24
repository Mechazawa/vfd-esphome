import pytest
from unittest.mock import MagicMock

from components.vfd_hcs_12ss59t import vfd_hcs_12ss59t

class DummyVFD:
    def __init__(self):
        self.displayed_text = ""
        self.brightness = 4

    def write_text(self, text):
        self.displayed_text = text

    def set_brightness(self, value):
        self.brightness = vfd_hcs_12ss59t.clamp_brightness(value)

@pytest.fixture
def vfd():
    return DummyVFD()

def test_display_text(vfd):
    vfd.write_text("HELLO")
    assert vfd.displayed_text == "HELLO"

def test_runtime_brightness_adjustment(vfd):
    vfd.set_brightness(10)
    assert vfd.brightness == 10
    vfd.set_brightness(20)
    assert vfd.brightness == 15
    vfd.set_brightness(-3)
    assert vfd.brightness == 0