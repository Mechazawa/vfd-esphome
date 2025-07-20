import importlib.util
import os
actions_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../components/vfd_hcs_12ss59t/actions.py'))
spec = importlib.util.spec_from_file_location("vfd_actions", actions_path)
vfd_actions = importlib.util.module_from_spec(spec)
spec.loader.exec_module(vfd_actions)

def test_write_action_schema():
    config = {"id": "vfd_display", "text": "Test123"}
    validated = vfd_actions.VFD_WRITE_ACTION_SCHEMA(config)
    assert validated["text"] == "Test123"

def test_clear_action_schema():
    config = {"id": "vfd_display"}
    validated = vfd_actions.VFD_CLEAR_ACTION_SCHEMA(config)
    assert validated["id"] == "vfd_display"
