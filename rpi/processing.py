from __main__ import app, interface
from flask import request

layerStart = {0: interface.scrollingLayer1_start,
              1: interface.scrollingLayer2_start,
              2: interface.scrollingLayer3_start,
              3: interface.scrollingLayer4_start,
              4: interface.scrollingLayer5_start}


@app.route("/", methods=["GET", "POST"])
def index():
    errors = ""
    layerText = ["" for i in range(5)]
    if request.method == "POST":
        for i in range(5):
            try:
                layerText[i] = str(request.form["layer"+str(i)])
            except:
                errors += "<p>{!r} is not a string.</p>\n".format(request.form["layer"+str(i)])
            if layerText[i] is not None and layerText[i] is not "":
                layerStart[i](layerText[i].encode('utf-8'), -1)
    return '''
        <html>
            <body>
                {errors}
                <p>Enter your layers:</p>
                <form method="post" action=".">
                    <p><input name="layer0" /></p>
                    <p><input name="layer1" /></p>
                    <p><input name="layer2" /></p>
                    <p><input name="layer3" /></p>
                    <p><input name="layer4" /></p>
                    <p><input type="submit" value="Set layers" /></p>
                </form>
            </body>
        </html>
    '''.format(errors=errors)


@app.route("/layer1")
def layer1text():
    interface.scrollingLayer1_start(b'Layer 1 test', -1)

@app.route("/layer2")
def layer2text():
    interface.scrollingLayer2_start(b'layer 2 test', -1)
