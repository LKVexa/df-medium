import importlib.util
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
VM = next((ROOT / 'vm').iterdir())
sys.path.insert(0, str(ROOT / 'adapter'))
from dfabric.nodes import MediumNodeAdapter
from dfabric import AdapterRefusal
spec = importlib.util.spec_from_file_location('brim_ref', VM / 'independent/brim_ref.py')
brim = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = brim
spec.loader.exec_module(brim)


class PortableSecurity(unittest.TestCase):
    def test_output_path_escapes_refused_before_lowering(self):
        adapter = object.__new__(MediumNodeAdapter)
        with patch('dfabric.nodes.W.lowering_record') as lower:
            for unit in ('../escape', '/tmp/escape', 'C:/escape', r'..\escape',
                         'safe/../../escape', '', '.', '..', 'NUL', 'COM1.log',
                         'name.', 'injected\nrow', 'x' * 129, None):
                with self.subTest(unit=unit), self.assertRaises(AdapterRefusal):
                    adapter.submit_words([1], unit_id=unit)
            lower.assert_not_called()

    def test_shipped_image_and_every_truncation(self):
        image = next((VM / 'deploy').glob('*.brimg')).read_bytes()
        parsed = brim.parse_brim(image)
        self.assertEqual(parsed.isa_major, 4)
        for length in range(len(image)):
            with self.subTest(length=length), self.assertRaises(brim.VerifyError):
                brim.parse_brim(image[:length])

    def test_image_mutations_rejected(self):
        image = next((VM / 'deploy').glob('*.brimg')).read_bytes()
        for position in (0, 4, 6, 9, 14, 28, 32, 80):
            corrupt = bytearray(image)
            corrupt[position] ^= 0xff
            with self.subTest(position=position), self.assertRaises(brim.VerifyError):
                brim.parse_brim(bytes(corrupt))


if __name__ == '__main__':
    unittest.main()
