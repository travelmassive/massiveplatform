---
title: API
has_children: false
nav_order: 5
---

# API

## Massive Platform API

There is a basic API provided by the tm_api module. You can add your own API hooks in here. Currently the following (basic) read-only API methods are published.

- api/public/test - API test page
- api/public/stats - basic stats
- api/public/chapters_lat_lon - chapters lat and lon
- api/public/chapter_leaders - list of chapter leaders
- api/public/events - list of events
- api/public/chapter_json - get a JSON payload from a chapter

All results as returned as JSON.

For JSONP append ?callback=myfunction to the URL. 


