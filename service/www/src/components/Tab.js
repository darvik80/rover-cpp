import React from 'react';
import PropTypes from 'prop-types';

const propTypes = {
  title: PropTypes.string,
};

// Just wrapper for content
const Tab = ({ children }) => ({ children });

Tab.propTypes = propTypes;

export default Tab;
