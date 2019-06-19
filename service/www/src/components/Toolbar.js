import React from 'react';
import PropTypes from 'prop-types';
import { ButtonToolbar } from 'reactstrap';
import classNames from 'classnames';

const propTypes = {
  className: PropTypes.string,
};

const Toolbar = ({ className, children, ...attributes }) => {
  className = classNames('justify-content-between m-1', className);
  return (
    <ButtonToolbar className={className} {...attributes}>
      {children}
    </ButtonToolbar>
  );
};

Toolbar.propTypes = propTypes;

export default Toolbar;
