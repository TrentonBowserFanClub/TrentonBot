import type { Config } from "tailwindcss";

const config: Config = {
  content: [
    "./src/pages/**/*.{js,ts,jsx,tsx,mdx}",
    "./src/components/**/*.{js,ts,jsx,tsx,mdx}",
    "./src/app/**/*.{js,ts,jsx,tsx,mdx}",
  ],
  theme: {
    extend: {
      backgroundImage: {
        "gradient-radial": "radial-gradient(var(--tw-gradient-stops))",
        "gradient-conic":
          "conic-gradient(from 180deg at 50% 50%, var(--tw-gradient-stops))",
      },
    },
    colors: {
      red: "#ee6352",
      green: "#59cd90",
      blue: "#3fa7d6",
      yellow: "#fac05e",
      purple: "#b678e0",
      black: "#1e1e1e",
      white: "#f9f9f9",
      orange: "#f79d84"
    }
  },
  plugins: [],
};
export default config;
