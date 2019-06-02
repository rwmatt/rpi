import json

fname = 'clockesp.ipynb'

fp = open(fname, "r")
js = json.load(fp)
fp.close()

for c in js['cells']:
    c['outputs'] = []

fp = open(fname, "w")
json.dump(js, fp, indent=4, sort_keys=True)
fp.close()
