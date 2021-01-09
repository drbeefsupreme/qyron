from __main__ import app, interface
from flask import request, render_template

#form magic
from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField
from wtforms.validators import DataRequired

layerStart = {1: interface.scrollingLayer1_start,
              2: interface.scrollingLayer2_start,
              3: interface.scrollingLayer3_start,
              4: interface.scrollingLayer4_start,
              5: interface.scrollingLayer5_start}

layerSpeed = {1: interface.scrollingLayer1_speed,
              2: interface.scrollingLayer2_speed,
              3: interface.scrollingLayer3_speed,
              4: interface.scrollingLayer4_speed,
              5: interface.scrollingLayer5_speed}

class LayerForm(FlaskForm):
    layerText = StringField('text')
    layerSpeed = StringField('speed')
    submit = SubmitField('submit')

@app.route("/alllayers", methods=["GET", "POST"])
def allLayers():
    errors = ""
    layerText = ["" for i in range(5)]
    if request.method == "POST":
        for i in range(5):
            try:
                layerText[i] = str(request.form["layer"+str(i+1)])
            except:
                errors += "<p>{!r} is not a string.</p>\n".format(request.form["layer"+str(i+1)])
            if layerText[i] is not None and layerText[i] is not "":
                layerStart[i+1](layerText[i].encode('utf-8'), -1)

    return render_template('alllayers.html', errors=errors)

@app.route("/", methods=["GET", "POST"])
def index():
    form = LayerForm()
    return render_template('layerform.html', title='Layers', form=form)


@app.route("/layer1")
def layer1text():
    interface.scrollingLayer1_start(b'Layer 1 test', -1)

@app.route("/layer2")
def layer2text():
    interface.scrollingLayer2_start(b'layer 2 test', -1)
