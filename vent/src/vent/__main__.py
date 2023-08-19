import mastodon
import zmq
import json
import datetime as dt

class DateTimeEncoder(json.JSONEncoder):
    def default(self, o):
        if isinstance(o, dt.datetime):
            return o.isoformat()

class QueueingListener(mastodon.StreamListener):
    def __init__(self):
        super().__init__()
        self.ctx = zmq.Context()
        self.pub = self.ctx.socket(zmq.PUB)
        self.pub.bind("tcp://*:5556")

    def on_update(self, status):
        self.pub.send_string(str(status["id"]))

if __name__ == '__main__':
    m = mastodon.Mastodon(api_base_url="https://fosstodon.org")
    h = m.stream_public(QueueingListener())
