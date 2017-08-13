#!/usr/bin/python3

"""
The idea is to create hashtables for all characters present in latin1 encoding

We need the hashtables:
    latin1 -> utf8
    utf8_lower -> utf8_upper
    utf8_upper -> utf8_lower
    utf8 -> latin1

"""

def ts(strr):
    """Changes a string of the form b'\xc3' into the form "\xc3" """
    return str(strr)[1:].replace('\'', '\"')

# All the characters we're making hashes of
g_chars = [ i.to_bytes(1, 'big').decode('latin1') for i in range(128, 256) ]

# Class for building the hash for convertion between latin1 upper and lowercase
class Latin1_case:
    u2l_hash = [ -1 for i in range(128) ]
    l2u_hash = [ -1 for i in range(128) ]
    
    @staticmethod
    def hashing(char):
        # Works for both u2l (upper to lower) and l2u
        return char.encode('latin1')[0] - 128
    
    def printHashes(self):
        print("\n\nLatin1 U2L\n")
        print( ",".join([str(i[0]) for i in self.u2l_hash]))
        print("\n\nLatin1 L2U\n")
        print( ",".join([str(i[0]) for i in self.l2u_hash]))

    def __init__(self):
        for char in g_chars:
            idx = self.hashing(char)
            try:
                self.u2l_hash[idx] = char.lower().encode('latin1')
            except:
                self.u2l_hash[idx] = char.encode('latin1')
            
            try:
                self.l2u_hash[idx] = char.upper().encode('latin1')
            except:
                self.l2u_hash[idx] = char.encode('latin1')

        self.printHashes()

class Utf8_case:
    u2l_hash = [ [0,0] for i in range(320) ]
    l2u_hash = [ [0,0] for i in range(320) ]

    @staticmethod
    def hashing(char):
        # Works for both u2l and l2u
        return int.from_bytes(char.encode(), 'big') - 49792

    def printHashes(self):
        print("\n\nUTF8 U2L")
        print(",".join([ "{{{},{}}}".format(i[0],i[1]) for i in self.u2l_hash ]))
        print("\n\nUTF8 L2U")
        print(",".join([ "{{{},{}}}".format(i[0],i[1]) for i in self.l2u_hash ]))

    def __init__(self):
        for char in g_chars:
            idx = self.hashing(char)
            self.u2l_hash[idx] = char.lower().encode()
            self.l2u_hash[idx] = char.upper().encode()

        self.printHashes()

class Utf8_2_Latin1:
    Hash = [ [0,0] for i in range(320) ]
    
    @staticmethod
    def hashing(char):
        return int.from_bytes(char.encode(), 'big') - 49792

    def printHash(self):
        print(",".join( [ str(i[0]) for i in self.Hash] ))

    def __init__(self):
        for char in g_chars:
            idx = self.hashing(char)
            self.Hash[idx] = char.encode('latin1')

        self.printHash()

class Latin1_2_Utf8:
    Hash = [ 0 for i in range(128) ]

    @staticmethod
    def hashing(char):
        return char.encode('latin1')[0] - 128

    def printHash(self):
        print(",".join([ "{{{},{}}}".format(i[0],i[1]) for i in self.Hash ]))

    def __init__(self):
        for char in g_chars:
            idx = self.hashing(char)
            self.Hash[idx] = char.encode()

        self.printHash()

class FileCreator:
    text = """sera íóúáé ã ser - eis a qüestão.\n"""
    
    base = ''.join([ i.to_bytes(1, 'big').decode('latin1') for i in range(0, 256) ])
    text += ''.join([ i for i in base if i.isprintable() ])
    
    def __init__(self):
        print(self.text)

        with open('latin.txt', 'w', encoding='latin1') as fp:
            fp.write(self.text)

        with open('utf.txt', 'w', encoding='utf8') as fp:
            fp.write(self.text)

class Latin1_2_Ascii:
    Hash = [ 0 for i in range(128) ]

    @staticmethod
    def hashing(char):
        return char.encode('latin1')[0] - 128

    def printHash(self):
        print(",".join([ i for i in self.Hash ]))

    def __init__(self):
        j = 0
        while j < len(g_chars):
            i = g_chars[j]

            print('{' + i + '}', end=' ')
            idx = self.hashing(i)
            char = input()
            if char == '-1':
                j -= 1
                continue
            elif len(char) == 1:
                self.Hash[ idx ] = char
            else:
                self.Hash[ idx ] = '*'
            j += 1

        self.printHash()

if __name__ == '__main__':
    # Latin1_case()
    # Utf8_case()
    # Utf8_2_Latin1()
    # Latin1_2_Utf8()
    # FileCreator()
    Latin1_2_Ascii()
