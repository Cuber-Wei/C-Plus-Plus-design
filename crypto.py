from Crypto.Util.number import bytes_to_long, long_to_bytes, inverse
from os import getcwd

PATH = getcwd()


class RSA:
    def __init__(self) -> None:
        self.p = 9731982593497035191
        self.q = 9485231599576174999
        self.e = 0x10001
        self.n = self.p * self.q
        self.phi = (self.p - 1) * (self.q - 1)
        self.d = inverse(self.e, self.phi)

    def enc(self, data):
        self._m = bytes(data, encoding="utf8")
        self.m = bytes_to_long(self._m)
        self.c = pow(self.m, self.e, self.n)
        with open(PATH + r'\output.txt', 'w') as f:
            f.write(str(self.c))

    def dec(self, data):
        self.c = int(data)
        self._m = pow(self.c, self.d, self.n)
        self.m = str(long_to_bytes(self._m), encoding="utf-8")
        with open(PATH + r'\output.txt', 'w') as f:
            f.write(str(self.m))


if __name__ == "__main__":
    p = RSA()
    with open(PATH + r'\input.txt', 'r') as f:
        mod, data = f.read().split(',')
        if mod == '1':
            p.enc(data)
        elif mod == '2':
            p.dec(data)
