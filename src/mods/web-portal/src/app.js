require("dotenv").config();
const createError = require("http-errors");
const express = require("express");
const path = require("path");
const cookieParser = require("cookie-parser");
const logger = require("morgan");
const session = require("express-session");

const indexRouter = require("./routes/index");
const usersRouter = require("./routes/users");
const ormRouter = require("./routes/api/v1/orm/");
const apiRouter = require("./routes/api/v1/");
let sequelize = null;

const app_prefix = process.env.APP_PREFIX || "/td";
async function db() {
  const { Sequelize } = require("sequelize");

  // Option 1: Passing a connection URI
  sequelize = new Sequelize(process.env.DB_STRING); // Example for postgres

  try {
    await sequelize.authenticate();
    if (process.env.VERBOSE) {
      console.log("Connection has been established successfully.");
    }
    return true;
  } catch (error) {
    console.error("Unable to connect to the database:", error);
    return false;
  }
}
if (!db()) {
  console.error("Error connecting to the db");
  return;
}
let parent_state = {
  sequelize,
};

const app = express();
const dev = false;

// view engine setup
//app.set("views", path.join(__dirname, "views"));
//app.set("view engine", "jade");

app.use(logger("dev"));
app.use(express.json());
//app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
//app.use(express.static(path.join(__dirname, "public")));

const jwt = require(__dirname + "/auth/jwt/");
app.use(app_prefix, indexRouter);
app.use(`${app_prefix}/users`, [jwt.authenticate_middleware, usersRouter]);
//app.use(`${app_prefix}/api/v1`, [jwt.authenticate_middleware, apiRouter]);
app.use(`${app_prefix}/api/v1/orm/`, [ormRouter(parent_state)]);

// catch 404 and forward to error handler
app.use(function (req, res, next) {
  next(createError(404));
});

// error handler
app.use(function (err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get("env") === "development" ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render("error");
});
app.set("trust proxy", 1); // trust first proxy
app.use(
  session({
    secret: process.env.SESSION_SECRET,
    resave: false,
    saveUninitialized: true,
    cookie: { secure: dev === true },
  })
);

module.exports = app;
