import math
import tkinter as tk
#import tkinter

import ser

root = tk.Tk()
S = tk.Scrollbar(root)
T = tk.Text(root, height=40, width=80)

#serout = open("foo", "w")

p = ser.Port()

def get_lines():
    pos = T.index('insert')
    row = math.floor(float(pos))
    contents = T.get("1.0", "end-1c")
    lines = contents.splitlines()
    return row, lines

def send_line(event):
    row, lines = get_lines()
    ser.send(p, lines[row-1] + "\n")

def send_jar(event):
    row, lines = get_lines()
    #print("Entered send_jar")
    while True:
        if row > len(lines): break
        line = lines[row-1]
        if line == "%%": break;
        ser.send(p, line + "\n")
        row = row +1


S.pack(side=tk.RIGHT, fill=tk.Y)
T.pack(side=tk.LEFT, fill=tk.Y)
S.config(command=T.yview)
T.config(yscrollcommand=S.set)

T.bind('<Control-j>', send_jar)
T.bind('<Control-l>', send_line)
T.insert(tk.END, '')
#tk.mainloop()
#tk1 = tk.Tk()
while True:
    root.update_idletasks()
    root.update()
    #print("x")
    ser.do_output(p)
