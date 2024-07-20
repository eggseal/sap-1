import React from "react";
import ReactDOM from "react-dom/client";
import "./index.css";
import App from "./layout/App";
import Header from "./layout/Header";
import NavBar from "./layout/NavBar";

const root = ReactDOM.createRoot(document.getElementById("root") as HTMLElement);
root.render(
  <React.StrictMode>
    <Header />
    <NavBar />
    <App />
  </React.StrictMode>
);
