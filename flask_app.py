from flask import Flask, request, jsonify, render_template
import os
# works well.
app = Flask(__name__)

@app.route('/')
def index():
    return render_template('webhookindex.html')

@app.route('/webhook', methods=['POST'])
def webhook():
    data = request.get_json(silent=True)
    dataAction = data['queryResult']['action']
    if data['queryResult']['queryText'] == 'yes':
        reply = {
            "fulfillmentText": "Ok. Tickets booked successfully.",
        }
        return jsonify(reply)

    elif data['queryResult']['queryText'] == 'no':
        reply = {
            "fulfillmentText": "Ok. Booking cancelled.",
        }
        return jsonify(reply)

    else :
        reply = {
            "fulfillmentText": f"{dataAction} ok, fine.",
        }
        return jsonify(reply)

if __name__ == '__main__':
    app.run(debug = True)
