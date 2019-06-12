import math
import tkinter as tk

root = tk.Tk()
S = tk.Scrollbar(root)
T = tk.Text(root, height=40, width=80)

serout = open("foo", "w")

def send_line(event):
    #print('You pressed Ctl-S')
    pos = T.index('insert')
    #row = math.floor(pos)
    #row = str(math.floor(float(pos))) + ".0"
    row = math.floor(float(pos))
    print(row)
    contents = T.get("1.0", "end-1c")
    lines = contents.splitlines()
    print(lines[row-1] )
    serout.write(lines[row-1])

S.pack(side=tk.RIGHT, fill=tk.Y)
T.pack(side=tk.LEFT, fill=tk.Y)
S.config(command=T.yview)
T.config(yscrollcommand=S.set)

T.bind('<Control-l>', send_line)
T.insert(tk.END, '')
tk.mainloop()
