import sqlite3
from flask import Flask, render_template, request

app = Flask(__name__)
conn = sqlite3.connect('main.db')

def is_valid_card_id(cardid):
    return True

@app.route("/")
def index():
    return "Hello world"

@app.route("/checkin/<rid>")
def checkin(rid):
    """
    - check trang thai
    - topup page
    """
    # list_of_available_room = get_list_of_available_room()
    list_of_available_room = []
    if rid in list_of_available_room:
        return "Phòng {} đã có chủ".format(rid)
    else:
        # return "Phòng {} sẵn sàng, bạn hãy nạp tiền để sử dụng phòng này".format(rid)
        return render_template("checkin.html", rid=rid)

@app.route("/topup")
def topup(cardid=123456):
    if cardid == 123456:
        # c = conn.cursor()
        return "Nạp thẻ thành công, bạn đã có thể mở cửa. ID của bạn là 889977"
    else:
        return "Sai mã số thẻ rồi"

@app.route("/open")
def open():
    pass

if __name__ == "__main__":
    app.run(debug=True)
