import { useEffect } from 'react';
import { withRouter } from 'react-router-dom'

const propTypes    = {};
const defaultProps = {};

const ScrollToTop = ({ location, children }) => {

  useEffect(() => {
    window.scrollTo(0, 0);
  }, [location]);

  return children;

};

ScrollToTop.propTypes    = propTypes;
ScrollToTop.defaultProps = defaultProps;

export default withRouter(ScrollToTop);
