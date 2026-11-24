#!/usr/bin/env python3
"""Serves build/browser (only that folder) on 127.0.0.1 with caching off, so a rebuild is always
what the browser runs. Usage: python3 browser/serve.py [port=8777]"""
import functools
import http.server
import os
import sys

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'build', 'browser')


class NoCache(http.server.SimpleHTTPRequestHandler):
    extensions_map = {**http.server.SimpleHTTPRequestHandler.extensions_map, '.wasm': 'application/wasm', '.mjs': 'text/javascript'}

    def end_headers(self):
        self.send_header('Cache-Control', 'no-store')
        super().end_headers()


port = int(sys.argv[1]) if len(sys.argv) > 1 else 8777
handler = functools.partial(NoCache, directory=os.path.normpath(ROOT))
print(f'serving {os.path.normpath(ROOT)} at http://127.0.0.1:{port}/boot.html (no caching)')
http.server.ThreadingHTTPServer(('127.0.0.1', port), handler).serve_forever()
