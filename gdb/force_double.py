class StubFunction(gdb.Breakpoint):
    def __init__(self, address, function):
        super().__init__(address, gdb.BP_BREAKPOINT, internal=True)
        self.function = function

    def stop(self):
        self.function()
        return False


def hook():
    gdb.parse_and_eval("*0x02022B4C |= 0x800000 | 0x8 | 1")
    pass


# bp = StubFunction('*0x0800D366', hook)
