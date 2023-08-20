# Mastodata
A fun project to learn about message queues using data from Mastodon statuses.

# Project Structure
There are 3 main pieces.

## vent
[Link to `vent/README.md`](vent/README.md)

This Python app uses [Mastodon.py](https://github.com/halcy/Mastodon.py) to
stream public statuses and pushes them onto ZMQ.

## task
[Link to `task/README.md`](task/README.md)

A C program that pulls statuses from ZMQ, does some TBD data analysis, and
pushes the results to another ZMQ.

## sink

This TBD app pulls results from ZMQ and does some more stuff TBD.
