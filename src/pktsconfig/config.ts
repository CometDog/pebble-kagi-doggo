export default [
  {
    type: "section",
    items: [
      {
        type: "heading",
        defaultValue: "Theme",
      },
      {
        type: "toggle",
        messageKey: "darkMode",
        label: "Dark Mode",
        description: "Use light text on a dark background",
        defaultValue: false,
      },
    ],
  },
  {
    type: "submit",
    defaultValue: "Save",
  },
];
