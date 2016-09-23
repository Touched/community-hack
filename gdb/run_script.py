import subprocess
import tempfile
import os
import io
from asc import compiler, pokecommands

class Task():
    tasks_address = 0x03005090
    task_size = 0x28
    task_count = 16

    @classmethod
    def start(cls, inf, func_addr):
        free_task_id = Task.get_free(inf)
        first_task_id = Task.get_first(inf)

        # Build the task
        data = int.to_bytes(func_addr, 4, 'little') + \
               b'\x01\xFE' + \
               int.to_bytes(first_task_id, 1, 'little') + \
               b'\xFF' + \
               (b'\x00' * (cls.task_size - 8))

        # Write our task into memory
        inf.write_memory(cls.tasks_address + free_task_id * cls.task_size, data)

        # Change the old first task to point to ours as the previous task
        inf.write_memory(cls.tasks_address + first_task_id * cls.task_size + 5,
                         int.to_bytes(free_task_id, 1, 'little'))

    @classmethod
    def get_free(cls, inf):
        for task_id in range(cls.task_count):
            task_address = cls.tasks_address + task_id * cls.task_size
            in_use = bytes(inf.read_memory(task_address + 4, 1))[0]

            if not in_use:
                return task_id
        else:
            raise RuntimeError("No free tasks found")

    @classmethod
    def get_first(cls, inf):
        for task_id in range(cls.task_count):
            task_address = cls.tasks_address + task_id * cls.task_size
            read = bytes(inf.read_memory(task_address + 4, 2))
            in_use = read[0]
            prev = read[1]

            if in_use and prev == 0xFE:
                return task_id
        else:
            return 0xFF


def compile_line(line):
    return compiler.make_bytecode([
        compiler.SourceBlock(origin="", lines=[
            compiler.CleanLine(source_line=compiler.Line(file_name="", line_num=0, text=line),
                               items=line.split())
        ])
    ], pokecommands.pkcommands, False, False, [])[0][1]


class RunOWScript(gdb.Command):
    def __init__(self):
        super().__init__("pksc", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        inf = gdb.selected_inferior()

        # TODO: Find a free address
        address = 0x08800000

        bootstrap = (b'\x10\xb5\x04\x1c\x04\x48\x05\x4a\x00\xf0\x05\xf8' +
                     b'\x20\x1c\x04\x4a\x00\xf0\x01\xf8\x10\xbd\x10\x47' +
                     address.to_bytes(4, 'little') +
                     b'\xe5\x9a\x06\x08\x09\x75\x07\x08')

        # Build one-liner script and add padding
        script = compile_line(arg) + compile_line("end")
        script += (4 - (len(script) % 4)) * b'\x00'

        # Place data in ROM
        data = script + bootstrap
        inf.write_memory(address, data)

        # Run our bootstrap code as a task
        Task.start(inf, (address + len(script)) | 1)

RunOWScript()
