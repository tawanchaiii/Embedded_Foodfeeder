const express = require("express");
const app = express();
require("dotenv").config();
const Redis = require("ioredis");
const redis = new Redis({
    host: process.env.HOST_REDIS,
    port: 13859,
    password: process.env.PASS_REDIS,
    connectTimeout: 10000,
});

app.get("/", (req, res) => {
    res.json({ message: "Welcome food feeder system." });
});

app.get("/parameter", async (req, res) => {
    var color = await redis.get("color");
    var timeinterval = await redis.get("timeinterval");
    var feedtime = await redis.get("feedtime");
    if (!color || !timeinterval || !feedtime) {
        return res.status(500).json({ message: "some error on redis" });
    }
    return res.json({ color, timeinterval, feedtime });
});

app.listen(8000, () => {
        console.log("Application is running on port 8000");
});
